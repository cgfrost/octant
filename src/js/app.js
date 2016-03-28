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
  "<!DOCTYPE html>" +
"<html>" +
  "<head>" +
  "<title>Slate Example Configuration</title>" +
  "</head>" +
  "<body>" +
    "<h1 class='title'>Slate Example Configuration</h1>" +
    "<div class='item-container'>" +
      "<div class='item-container-content'>" +
        "<div class='item'>" +
          "Use this configuration page to choose the settings you would like applied to this app." +
        "</div>" +
      "</div>" +
    "</div>" +

    "<div class='item-container'>" +
      "<div class='item-container-header'>Customizability</div>" +
      "<div class='item-container-content'>" +
        "<label class='item'>" +
          "Background Color" +
          "<input id='background_color_picker' type='text' class='item-color item-color-sunny' value='#000000'>" +
        "</label>" +
      "</div>" +
      "<div class='item-container-footer'>" +
        "Choose the app's background color. Used if High Contrast Mode is not selected. Note: Only available on Basalt watches that support many colors." +
      "</div>" +
    "</div>" +
    "<div class='item-container'>" +
      "<div class='item-container-header'>Accessibility</div>" +
      "<div class='item-container-content'>" +
        "<label class='item'>" +
          "High Contrast Mode" +
          "<input id='high_contrast_checkbox' type='checkbox' class='item-toggle'>" +
        "</label>" +
      "</div>" +
      "<div class='item-container-footer'>" +
        "This switches the app colors to a higher contrast set that promotes readability in low light." +
      "</div>" +
    "</div>" +
    "<div class='item-container'>" +
      "<div class='button-container'>" +
        "<input id='submit_button' type='button' class='item-button' value='SAVE'>" +
      "</div>" +
    "</div>" +
  "</body>" +
  "<script>" +
  "function getConfigData() {" +
    "var backgroundColorPicker = document.getElementById('background_color_picker');" +
    "var highContrastCheckbox = document.getElementById('high_contrast_checkbox');" +
    "var options = {" +
      "'background_color': backgroundColorPicker.value," +
      "'high_contrast': highContrastCheckbox.checked" +
    "};" +
    "// Save for next launch" +
    "localStorage['background_color'] = options['background_color'];" +
    "localStorage['high_contrast'] = options['high_contrast'];" +
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

