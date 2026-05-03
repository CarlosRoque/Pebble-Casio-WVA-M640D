var DEFAULT_API_KEY = '04bab1d7cb93aaaff8c371bf864f9651';

var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

var weatherIcons = {
  '01d': 'I', '02d': '"', '03d': '!', '04d': 'k',
  '09d': '$', '10d': '+', '11d': 'F', '13d': '9', '50d': '=',
  '01n': 'N', '02n': '#', '03n': '!', '04n': 'k',
  '09n': '$', '10n': ',', '11n': 'F', '13n': '9', '50n': '>'
};

function fetchWeather(apiKey) {
  navigator.geolocation.getCurrentPosition(
    function(pos) {
      var lat = pos.coords.latitude.toFixed(3);
      var lon = pos.coords.longitude.toFixed(3);
      var url = 'http://api.openweathermap.org/data/2.5/weather' +
                '?lat=' + lat + '&lon=' + lon +
                '&units=metric&appid=' + apiKey;
      var req = new XMLHttpRequest();
      req.open('GET', url, true);
      req.onload = function() {
        if (req.status === 200) {
          var data = JSON.parse(req.responseText);
          Pebble.sendAppMessage({
            'W_ICON': weatherIcons[data.weather[0].icon] || 'h',
            'W_TEMP': Math.round(data.main.temp),
            'W_COND': data.weather[0].description
          },
          function() { console.log('Weather sent OK'); },
          function(e) { console.log('Weather send failed: ' + JSON.stringify(e)); });
        } else {
          console.log('OWM error: ' + req.status);
        }
      };
      req.send();
    },
    function(err) { console.log('Geolocation error: ' + err.message); },
    { timeout: 15000, maximumAge: 300000 }
  );
}

function boolSetting(val, defaultVal) {
  if (val === undefined || val === null) return defaultVal ? 1 : 0;
  return (val === true || val === 'true' || val === '1' || parseInt(val) === 1) ? 1 : 0;
}

Pebble.addEventListener('ready', function() {
  var settings = JSON.parse(localStorage.getItem('clay-settings') || '{}');
  var apiKey = settings.OWM_API_KEY || DEFAULT_API_KEY;
  var units = boolSetting(settings.S_USE_FAHRENHEIT, false);
  var vibBt = boolSetting(settings.S_VIB_BT, true);
  var battDisp = boolSetting(settings.S_BATT_SHOW_PCT, false);

  Pebble.sendAppMessage({ 'S_USE_FAHRENHEIT': units, 'S_VIB_BT': vibBt, 'S_BATT_SHOW_PCT': battDisp },
    function() { fetchWeather(apiKey); },
    function() { fetchWeather(apiKey); }
  );
});
