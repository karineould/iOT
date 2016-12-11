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
    var cpu, th;

    //Socket client creation
    var client = new net.Socket();

    //socket client connect to raspberry server
    client.connect(port, ip, function (data) {
        console.log('CONNECTED TO: ' + ip + ':' + port);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
        client.write('GET');
    });

    // Add a 'data' event handler for the client socket
    // data is what the server sent to this socket
    var io = req.app.get('socketio');
    client.on('data', function (data) {
        console.log('DATA: ' + data);

        var splitData = data.toString().split(";");
        var cpuData = splitData[0].split("=");
        var thData = splitData[1].split("=");
        cpu = cpuData[1];
        th = thData[1];

        io.emit('newData',{cpu:cpu,thread:th});

        if(io.connectedStatus==false){
            console.log('Destruction du socket');
            client.destroy();
        }

    });
    // Add a 'close' event handler for the client socket
    client.on('close', function() {
        console.log('Connection closed');
    });

    res.render('data.twig', { title: 'RECEIVE DATA FROM RASPBERRY' });

});

module.exports = router;
