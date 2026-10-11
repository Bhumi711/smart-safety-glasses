
# ESP32-S3 Hardware Pin Mapping

Board: UG LAND INDIA ESP32-S3 WROOM N16R8 + OV3660

Pin assignments must be verified against the exact board schematic
before connecting any peripherals.

| Component | Signal | GPIO | Status |
|---|---|---|---|
| SOS button | Digital input | TBD | Pending |
| NEO-6M GPS | UART RX | TBD | Pending |
| NEO-6M GPS | UART TX | TBD | Pending |
| INMP441 | I2S BCLK | TBD | Pending |
| INMP441 | I2S WS | TBD | Pending |
| INMP441 | I2S SD | TBD | Pending |
| Camera | Camera signals | Board-specific | Pending |
| microSD | SPI/SD signals | Board-specific | Pending |

## Notes

- Do not assign GPIOs until camera and microSD wiring is verified.
- Confirm voltage requirements before connecting modules.
- Test each peripheral independently before full integration.
