var express = require('express');
var router = express.Router();
var net = require('net');
var io = require('socket.io');


/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index.twig', { title: 'Temps réel' });
});


router.get('/data', function(req, res){
    var ip = req.query.ip;
    var port = req.query.port;
    var cpu, proc, memUsed, memfree;

    //Socket client creation
    var client = new net.Socket();

    console.log(port + ' '+ ip);
    //socket client connect to raspberry server
    client.connect(port, ip, function () {
        console.log('CONNECTED TO: ' + ip + ':' + port);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
        client.write('GET');
        res.render('data.twig', { title: 'RECEIVE DATA FROM RASPBERRY' });
    });

    client.on('error', function (err) {
        console.log('error : '+ err);
        client.destroy();
        res.render('index.twig', { title: 'Temps réel', errorConnectRap:true });
    });
    // Add a 'data' event handler for the client socket
    // data is what the server sent to this socket
    var io = req.app.get('socketio');
    client.on('data', function (data) {
        console.log('DATA: ' + data);

        var splitData = data.toString().split(";");
        var cpuData = splitData[0].split("=");
        var procData = splitData[1].split("=");
        var memUsedData = splitData[2].split("=");
        var memFreeData = splitData[3].split("=");
        cpu = cpuData[1];
        proc = procData[1];
        memUsed = memUsedData[1];
        memfree = memFreeData[1];

        console.log(io.connectedStatus);
        if(io.connectedStatus==null || io.connectedStatus==true) {
            io.emit('newData', {cpu: cpu, proc: proc, memUsed: memUsed, memFree: memfree});
        } else {
            console.log('Destruction du socket');
            client.destroy();
        }
    });

    // Add a 'close' event handler for the client socket
    client.on('close', function() {
        console.log('Connection closed');
    });

});

module.exports = router;
