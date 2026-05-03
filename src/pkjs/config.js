module.exports = [
  {
    type: "heading",
    defaultValue: "WVA-M640D by Carlos Roque",
  },
  {
    type: "input",
    messageKey: "OWM_API_KEY",
    label: "OpenWeatherMap API Key",
    description:
      "It is recomended that you get your own free key at openweathermap.org/api",
    defaultValue: "04bab1d7cb93aaaff8c371bf864f9651",
  },
  {
    type: "select",
    messageKey: "S_USE_FAHRENHEIT",
    label: "Temperature Units",
    defaultValue: "0",
    options: [
      { label: "Celsius (°C)", value: "0" },
      { label: "Fahrenheit (°F)", value: "1" },
    ],
  },
  {
    type: "heading",
    defaultValue: "Battery",
  },
  {
    type: "select",
    messageKey: "S_BATT_SHOW_PCT",
    label: "Battery Display",
    defaultValue: "0",
    options: [
      { label: "Icon", value: "0" },
      { label: "Percentage", value: "1" },
    ],
  },
  {
    type: "heading",
    defaultValue: "Notifications",
  },
  {
    type: "toggle",
    messageKey: "S_VIB_BT",
    label: "Vibrate on Bluetooth Disconnect",
    defaultValue: true,
  },
  {
    type: "submit",
    defaultValue: "Save Settings",
  },
  {
    type: "text",
    defaultValue: "WVA-M640D v1.0",
  },
];
