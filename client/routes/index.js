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
    var donnee;
    client.on('data', function (data) {

        donnee=data;
        console.log('DATA: ' + data);
        // Close the client socket completely
        // client.destroy();

    });
    // Add a 'close' event handler for the client socket
    client.on('close', function() {
        console.log('Connection closed');
    });

  res.render('data', { title: 'RECEIVE DATA', data: donnee});
});

module.exports = router;
