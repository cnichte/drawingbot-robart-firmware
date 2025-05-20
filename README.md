# Robart Firmware

- This is the firmware for the Omnidirectional Pen Plotter [Robart](https://github.com/cnichte/drawingbot-robart).
- Developed under VS-Code and PlatformIO.
- This project is licensed under the GNU General Public License v3.0. See the LICENSE file for details.

---

## RobArt_Bluetooth_Parser

RobArt_Bluetooth_Parser unterstützt die GCode-Befehle **G1**, **M3**, **M5** und **M105**, die für Bewegung, Motor-Steuerung und Statusabfragen.

- **G1**: Linearbewegung zu einer bestimmten Position.
  - Extrahiert die X- und Y-Werte aus dem Befehl (z. B. G1 X100 Y200).
  - Ruft die moveCb-Callback-Funktion auf, wenn vorhanden, mit den Parametern x und y.
  - Fallback: Ruft moveTo(x, y) auf, wenn kein Callback definiert ist, und gibt die Bewegungsinformationen über den Stream aus.

- **M3**: Aktiviert einen Motor mit PWM (Pulsweitenmodulation).
  - Extrahiert den PWM-Wert aus dem Parameter S (z. B. M3 S150).
  - Ruft die ledCb-Callback-Funktion auf, wenn vorhanden, mit true (an) und dem PWM-Wert.
  - Fallback: Setzt den PWM-Wert direkt auf Pin 3 und schaltet die LED an Pin 13 ein.

- **M5**: Deaktiviert einen Motor.
  - Ruft die ledCb-Callback-Funktion auf, wenn vorhanden, mit false (aus) und PWM-Wert 0.
  - Fallback: Setzt den PWM-Wert auf Pin 3 auf 0 und schaltet die LED an Pin 13 aus.
- **M105**: Statusbericht anfordern.
  - Ruft die statusCb-Callback-Funktion auf, wenn vorhanden.
  - Fallback: Gibt eine Standardantwort OK T:24.5 über den Stream aus (vermutlich eine Temperatur oder ein Status).