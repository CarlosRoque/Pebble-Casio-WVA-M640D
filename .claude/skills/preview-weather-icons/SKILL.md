---
name: preview-weather-icons
description: Generate a visual preview of all weather icons used by the watchface. Reads the icon mapping from src/pkjs/index.js and renders each Climacons font character into a labeled PNG grid. Use when the user says "preview weather icons", "show weather icons", "render icons", or "icon preview".
---

# Preview Weather Icons

Generate a visual preview image showing every weather icon the watchface can display, rendered from the Climacons font with labels.

## Steps

1. Read the weather icon mapping from `src/pkjs/index.js` (the `weatherIcons` object).
2. Run the Python script below to render all mapped characters from the Climacons font into a labeled grid.
3. Display the resulting image to the user.

## Script

Run this Python script from the project root. It requires `Pillow` and `fontTools` (install with `pip install Pillow fonttools` if missing).

```python
from PIL import Image, ImageFont, ImageDraw

font_path = "resources/fonts/Climacons.ttf"
font = ImageFont.truetype(font_path, 80)

# Weather icon mapping from src/pkjs/index.js
# Update this list if the mapping in index.js changes.
icons = [
    ("I",  "01d",   "Clear Sky (Day)"),
    ("N",  "01n",   "Clear Sky (Night)"),
    ('"',  "02d",   "Few Clouds (Day)"),
    ("#",  "02n",   "Few Clouds (Night)"),
    ("!",  "03d/n", "Scattered Clouds"),
    ("k",  "04d/n", "Overcast Clouds"),
    ("$",  "09d/n", "Drizzle"),
    ("+",  "10d",   "Rain (Day)"),
    (",",  "10n",   "Rain (Night)"),
    ("F",  "11d/n", "Thunderstorm"),
    ("9",  "13d/n", "Snow"),
    ("=",  "50d",   "Mist (Day)"),
    (">",  "50n",   "Mist (Night)"),
    ("h",  "??",    "Fallback / Unknown"),
]

cols = 2
rows = (len(icons) + cols - 1) // cols
cell_w = 400
cell_h = 120
padding = 10

img_w = cols * cell_w
img_h = rows * cell_h
img = Image.new("RGB", (img_w, img_h), "white")
draw = ImageDraw.Draw(img)

# Try common system fonts for labels
for label_path in [
    "/usr/share/fonts/google-noto/NotoSans-Regular.ttf",
    "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/TTF/DejaVuSans.ttf",
]:
    try:
        label_font = ImageFont.truetype(label_path, 18)
        break
    except OSError:
        continue
else:
    label_font = ImageFont.load_default()

for bold_path in [
    "/usr/share/fonts/google-noto/NotoSans-Bold.ttf",
    "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
    "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
]:
    try:
        code_font = ImageFont.truetype(bold_path, 16)
        break
    except OSError:
        continue
else:
    code_font = ImageFont.load_default()

for i, (char, code, desc) in enumerate(icons):
    col = i % cols
    row = i // cols
    x = col * cell_w + padding
    y = row * cell_h + padding

    draw.rectangle(
        [x - padding, y - padding, x + cell_w - padding, y + cell_h - padding],
        outline="#ddd", width=1,
    )
    draw.text((x + 5, y + 5), char, font=font, fill="black")

    label_x = x + 95
    draw.text((label_x, y + 10), f"char: '{char}'   OWM: {code}", font=code_font, fill="#555")
    draw.text((label_x, y + 35), desc, font=label_font, fill="black")

out_path = "resources/images/weather_icons_preview.png"
img.save(out_path)
print(f"Saved preview to: {out_path}")
```

## Notes

- The output is saved to `resources/images/weather_icons_preview.png`.
- The icon-to-character mapping comes from the OpenWeatherMap icon codes (e.g. `01d` = clear day, `10n` = rain night).
- If new weather conditions are added to `src/pkjs/index.js`, update the `icons` list in this script to match.
- The `characterRegex` in `package.json` controls which Climacons glyphs get included in the Pebble build: `[I"!k$+F9=N#,>h]`.
