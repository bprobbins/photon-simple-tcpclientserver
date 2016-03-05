//useful for testing server
"use strict";

var net = require('net');

var HOST = 'XXX.XXX.XXX.XXX';//ip of your server

var PORT = 6123;

var loopCounter = 0


var replymsg = 'TheInMsg and then a whole lot more characters than before';
var clientmsg = 'mymsg 2 and then a whole lot more characters than before';

var recursive = function () {

   var client = new net.Socket();

    client.connect(PORT, HOST, function () {

    client.write(clientmsg);

    // Add a 'data' event handler for the client socket
    // data is what the server sent to this socket
    client.on('data', function (data) {

    console.log('Response received: ' + data);

    // Close the client socket completely
    client.destroy();

  });


      client.on('error', function (err)  {
        console.log('Socket error: ');
        console.log(err.stack);
      });

        console.log('CONNECTED TO: ' + HOST + ':' + PORT);

    // Add a 'close' event handler for the client socket
    client.on('close', function () {
            console.log('Connection closed');
            loopCounter++;
            console.log('Loop #: ' + loopCounter)
        });

        setTimeout(recursive, 150);

    })

}

recursive();
