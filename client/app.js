var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var routes = require('./routes/index');
var users = require('./routes/users');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', routes);
app.use('/users', users);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});


// socket.io

var server = require('http').Server(app);

app.get('/', function (req, res) {
  //res.writeHead(
  //   206,
  //   {
  //   'Access-Control-Allow-Credentials': true,
  //   'Access-Control-Allow-Origin': true,
  //   }
  // );
  res.sendfile(__dirname + '/views/index.jade');
});


// Chargement de socket.io
var io = require('socket.io').listen(server);

// Quand un client se connecte, on le note dans la console
io.sockets.on('connection', function (socket) {
  console.log('Un client est connecté !');
  socket.emit('connexion', "Vous êtes bien connecté");

  //socket.emit('sendData', cpu, thread);
  setInterval(function(){
    var random1 = Math.floor((100)*Math.random()+0) ;
    var random2 = Math.floor((100)*Math.random()+0) ;
    socket.emit('sendData', random1, random2);
  }, 1000);
});


server.listen(3001);

module.exports = app;