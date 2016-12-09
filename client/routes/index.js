var express = require('express');
var router = express.Router();
var net = require('net');


/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Temps réel' });
});


router.get('/data', function(req, res){
    var ip = req.query.ip;
    var port = req.query.port;

    var client = new net.Socket();
    client.connect(port, ip, function (data) {

        console.log('CONNECTED TO: ' + ip + ':' + port);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
        client.write('GET');

    });
    // Add a 'data' event handler for the client socket
    // data is what the server sent to this socket
    var cpu, th;

    client.on('data', function (data) {
        var donnee = data;
        var splitData = data.toString().split(";");
        var cpuData = splitData[0].split("=");
        var thData = splitData[1].split("=");
        cpu = cpuData[1];
        th = thData[1];
        console.log('DATA: ' + data);
        console.log('cpu = '+ cpuData[1]+' thread = '+thData[1]);
        // Close the client socket completely
        // client.destroy();
        //res.render('data', { title: 'RECEIVE DATA', cpu: cpu, th: th});
        $('#gaugeCPU').refresh(cpu);
        $('#gaugeThread').refresh(th);

    });
    // Add a 'close' event handler for the client socket
    client.on('close', function() {
        console.log('Connection closed');
    });

    //res.send('test');//
    res.render('data', { title: 'RECEIVE DATA FROM RASPBERRY' });

});

module.exports = router;
