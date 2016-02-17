var net = require('net');
var dgram = require('dgram');
var port = 2000;
var now = function() {
  var date = new Date();
  return new Buffer(date.toUTCString() + "\r\n");
}

var connections = [];
var tcpserver = net.createServer(function(c) {
  c.on('data', function(chunk) {
  	console.log('chunk', chunk);
  	console.log('writing resposne to tcp');
  	c.write(now());
  });
  connections.push(c);
  //c.end();
});
tcpserver.listen(port);

var udpserver = dgram.createSocket("udp4", function(msg, rinfo) {
  console.log('message', msg);
  console.log('writing resposne to tcp');
  var daytime = now();
  udpserver.send(daytime, 0, daytime.length, rinfo.port, rinfo.address);
});
udpserver.bind(port);