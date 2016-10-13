var Clay = require('pebble-clay');

var clayConfig = [
  {
    "type": "heading",
    "defaultValue": "Octant Configuration"
  },
  {
    "type": "text",
    "defaultValue": "A simple watch face taking it's style from Jolla."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Settings"
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_STEPS",
        "label": "Show daily step count",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "SHOW_TAP_SCREEN",
        "label": "Shake to show 2nd screen",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];

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
