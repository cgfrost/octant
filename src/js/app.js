// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
  for (var key in Pebble) {
    if (Pebble.hasOwnProperty(key)) {
      console.log("Pebble - Own - " + key);
    } else {
      console.log("Pebble - Not - " + key);
    }
  }
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
    console.log('AppMessage received!');
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('Web view closed!'); 
//   var options = JSON.parse(decodeURIComponent(e.response));
//   console.log("Options = " + JSON.stringify(options));
  console.log("Options = " + e.response);
});

Pebble.addEventListener('showConfiguration', function() {
  console.log('Show Configuration!');
  Pebble.openURL('data:text/html,<html><body>' + 
                  '<h1>Hello World</h1>' +
                 '</body></html><!--.html');
});

