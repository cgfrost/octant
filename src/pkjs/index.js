var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

// Pebble.addEventListener('ready', function(e) {
//   console.log('PebbleKit JS ready!');
// });

// Pebble.addEventListener('appmessage', function(e) {
//     console.log('AppMessage received!');
// });

// Pebble.addEventListener('webviewclosed', function(e) {
//   console.log('Web view closed!'); 
// //   var options = JSON.parse(decodeURIComponent(e.response));
// //   console.log("Options = " + JSON.stringify(options));
//   console.log("Options = " + e.response);
// });

// Pebble.addEventListener('showConfiguration', function() {
//   console.log('Show Configuration!'); 
// });
