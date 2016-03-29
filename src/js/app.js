// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
//   for (var key in Pebble) {
//     if (Pebble.hasOwnProperty(key)) {
//       console.log("Pebble - Own - " + key);
//     } else {
//       console.log("Pebble - Not - " + key);
//     }
//   }
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
  Pebble.openURL("data:text/html," +   
  "<!DOCTYPE html><html>" +
  "<head><title>Octant Configuration</title></head>" +
  "<body>" +
    "<h1 class='title'>Octant Configuration</h1>" +
      "<input id='vibrate_on_disconnect_checkbox' type='checkbox'>" +
        "Vibrate on disconnect" +
      "</input>" +     
      "<input id='submit_button' type='button' value='Save'>" +
    "</body>" +
  "<script>" +
  "function getConfigData() {" +
    "var vibrateOnDisconnectCheckbox = document.getElementById('vibrate_on_disconnect_checkbox');" +
    "var options = {" +
      "'vibrate_on_disconnect_checkbox': vibrateOnDisconnectCheckbox.value," +
    "};" +
    "localStorage['vibrate_on_disconnect_checkbox'] = options['vibrate_on_disconnect_checkbox'];" +      
    "console.log('Got options: ' + JSON.stringify(options));" +
    "return options;" +
  "}" +
                 
                 
                 
                 
  "function getQueryParam(variable, defaultValue) {" +
    "var query = location.search.substring(1);" +
    "var vars = query.split('&');" +
    "for (var i = 0; i < vars.length; i++) {" +
      "var pair = vars[i].split('=');" +
      "if (pair[0] === variable) {" +
        "return decodeURIComponent(pair[1]);" +
      "}" +
    "}" +
    "return defaultValue || false;" +
  "}" +
  "var submitButton = document.getElementById('submit_button');" +
  "submitButton.addEventListener('click', function() {" +
    "console.log('Submit');" +
    "// Set the return URL depending on the runtime environment" +
    "var return_to = getQueryParam('return_to', 'pebblejs://close#');" +
    "document.location = return_to + encodeURIComponent(JSON.stringify(getConfigData()));" +
  "});" +
  "(function() {" +
    "var backgroundColorPicker = document.getElementById('background_color_picker');" +
    "var highContrastCheckbox = document.getElementById('high_contrast_checkbox');" +
    "// Load any previously saved configuration, if available" +
    "if(localStorage['high_contrast']) {" +
      "highContrastCheckbox.checked = JSON.parse(localStorage['high_contrast']);" +
      "backgroundColorPicker.value = localStorage['background_color'];" +
    "}" +
  "})();" +
                 
                 
                 
                 
  "</script>" +
"</html><!--.html");
  
});

