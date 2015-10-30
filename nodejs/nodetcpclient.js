//useful for testing server
"use strict";

var net = require('net');

var HOST = '192.168.1.10';

var PORT = 6123;

var loopCounter = 0

var recursive = function () {

    var client = new net.Socket();

    client.connect(PORT, HOST, function () {

      client.on('error', function (err)  {
        console.log('Socket error: ');
        console.log(err.stack);
      });

        console.log('CONNECTED TO: ' + HOST + ':' + PORT);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
        client.write('x');

        // Add a 'data' event handler for the client socket
        // data is what the server sent to this socket
        client.on('data', function (data) {

            console.log('Response received: ' + data);
            // Close the client socket completely
          client.destroy();

        });

    // Add a 'close' event handler for the client socket
    client.on('close', function () {
            console.log('Connection closed');
            loopCounter++;
            console.log('Loop #: ' + loopCounter)
        });

        setTimeout(recursive, 100);

    })

}

recursive();
