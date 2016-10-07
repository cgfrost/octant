module.exports = [
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
        "defaultValue": "More Settings"
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
