// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
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

// Pebble.addEventListener('showConfiguration', function() {
//   console.log('Show Configuration!');
//   Pebble.openURL("data:text/html," +   
//   "<!DOCTYPE html><html>" +
//   "<head><title>Octant Configuration</title></head>" +
//   "<body>" +
//     "<h1 class='title'>Octant Configuration</h1>" +
//       "<input id='vibrate_on_disconnect_checkbox' type='checkbox'>Vibrate on disconnect</input>" +     
//       "<input id='submit_button' type='button' value='Save'>" +
//     "</body>" +
//   "<script>" +
//   "function getConfigData() {" +
//     "var vibrateOnDisconnectCheckbox = document.getElementById('vibrate_on_disconnect_checkbox');" +
//     "var options = {" +
//       "'vibrate_on_disconnect_checkbox': vibrateOnDisconnectCheckbox.value," +
//     "};" +
//     "localStorage['vibrate_on_disconnect_checkbox'] = options['vibrate_on_disconnect_checkbox'];" +      
//     "console.log('Got options: ' + JSON.stringify(options));" +
//     "return options;" +
//   "}" +
//   "var submitButton = document.getElementById('submit_button');" +
//   "submitButton.addEventListener('click', function() {" +
//     "console.log('Submit ' + encodeURIComponent(JSON.stringify(getConfigData()));" +
//     "document.location = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(getConfigData()));" +
//   "});" +
//   "(function() {" +
//     "var vibrateOnDisconnectCheckbox = document.getElementById('vibrate_on_disconnect_checkbox');" +
//     "// Load any previously saved configuration, if available" +
//     "if(localStorage['vibrate_on_disconnect_checkbox']) {" +
//       "vibrateOnDisconnectCheckbox.checked = JSON.parse(localStorage['vibrate_on_disconnect_checkbox']);" +
//     "}" +
//   "})();" +        
//   "</script>" +
// "</html><!--.html");
// });

