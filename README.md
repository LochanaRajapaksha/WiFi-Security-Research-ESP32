ESP32 Captive Portal - Educational Security Demo
=================================================

⚠️ IMPORTANT DISCLAIMER - READ BEFORE USING ⚠️

This project is created STRICTLY FOR EDUCATIONAL PURPOSES ONLY. The techniques demonstrated here are for understanding Wi-Fi security vulnerabilities and developing defense mechanisms against such attacks.

DO NOT use this on networks you do not own or have explicit written permission to test. Unauthorized access to computer systems is ILLEGAL and punishable by law. The author assumes NO LIABILITY for any misuse of this code.

================================================================================
WHAT IS THIS?
================================================================================

This project demonstrates how an Evil Twin Attack works using an ESP32 microcontroller. An Evil Twin is a fraudulent Wi-Fi access point that appears to be legitimate, designed to trick users into connecting and revealing sensitive information.

The Attack Flow:
1. ESP32 creates a fake Wi-Fi network with a real-looking name
2. Victim connects to the fake network
3. Victim sees a login page that looks authentic
4. When victim enters credentials, they are captured
5. Victim is redirected to the real internet (seems normal)

================================================================================
FEATURES
================================================================================

| Feature              | Description                                              |
|----------------------|----------------------------------------------------------|
| Fake Wi-Fi AP        | Creates customizable Wi-Fi network                      |
| Captive Portal       | Professional login page (customizable HTML/CSS)         |
| Credential Capture   | Saves username/password to SPIFFS                       |
| Admin Panel          | Web interface to view captured credentials at /admin    |
| Persistent Storage   | Credentials saved even after power loss                 |
| Mobile Responsive    | Works on phones, tablets, and computers                 |
| Serial Monitor       | Real-time credential display                            |
| DNS Spoofing         | Redirects all traffic to portal                         |

================================================================================
HARDWARE REQUIREMENTS
================================================================================

- ESP32 Board (any development board: DevKit C, NodeMCU-32S, etc.)
- USB Data Cable (not just charging cable)
- USB port or 5V battery for power

Compatible ESP32 Boards:
- ESP32 DevKit V1
- NodeMCU-32S
- ESP32-WROOM-32
- ESP32-CAM (without camera)
- LOLIN32
- Any ESP32 with WiFi capability

================================================================================
SOFTWARE REQUIREMENTS
================================================================================

- Arduino IDE (version 1.8.10 or higher)
- ESP32 Board Package (version 2.0.0 or higher)
- USB-to-Serial Driver (CH340 or CP2102 depending on your board)

Required Arduino Libraries (included with ESP32 package):
- WiFi.h
- DNSServer.h
- WebServer.h
- SPIFFS.h

================================================================================
INSTALLATION GUIDE
================================================================================

Step 1: Install Arduino IDE
Download and install Arduino IDE from arduino.cc

Step 2: Add ESP32 Board
1. Open Arduino IDE -> File -> Preferences
2. Add to "Additional Boards Manager URLs":
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Tools -> Board -> Boards Manager
4. Search "ESP32" -> Install "esp32 by Espressif Systems"

Step 3: Install USB Driver (if needed)
- For CH340 chip: Download CH340 driver
- For CP2102 chip: Download CP2102 driver

Step 4: Configure Settings
Open the .ino file and modify:
   const char* evil_ssid = "FREE_WIFI";     // Change to your desired network name
   const char* ap_password = "";             // Leave empty for open network

Step 5: Upload to ESP32
1. Connect ESP32 via USB
2. Select board: Tools -> Board -> ESP32 Dev Module
3. Select port: Tools -> Port -> (your COM port)
4. Click Upload (->) button

Step 6: Open Serial Monitor
Tools -> Serial Monitor (Baud rate: 115200)

================================================================================
HOW TO USE
================================================================================

Starting the Attack:
1. Upload code to ESP32
2. Open Serial Monitor - You'll see:
   [+] ESP32 Evil Twin Started
   [+] Fake AP: FREE_WIFI
   [+] IP: 192.168.4.1
   [+] Admin panel: http://192.168.4.1/admin
   [+] Waiting for victims...

As a Victim (Testing on yourself):
1. Scan Wi-Fi on your phone/computer
2. Connect to "FREE_WIFI" (no password)
3. Open browser -> Any website
4. Login page appears - Enter any credentials
5. Redirected to Google/Yahoo

As an Attacker (Viewing Captured Credentials):

Method 1 - Serial Monitor:
Watch real-time as victims log in. Press 'c' in Serial Monitor to view all saved credentials.

Method 2 - Admin Panel (Browser):
Open http://192.168.4.1/admin to see table of all captured credentials with refresh and clear buttons.

Method 3 - SPIFFS File:
Credentials are saved to /creds.txt on ESP32 flash memory.

================================================================================
CODE STRUCTURE
================================================================================

The code is organized as follows:

1. Libraries and Configuration
   #include <WiFi.h>
   #include <DNSServer.h>
   #include <WebServer.h>
   #include <SPIFFS.h>

2. HTML Pages
   const char* login_page = R"rawliteral(...)rawliteral";
   const char* success_page = R"rawliteral(...)rawliteral";

3. Credential Handling
   void save_credentials(String user, String pass) {...}

4. Setup - AP Creation
   void setup() {
     WiFi.softAP(ssid, password);
     dnsServer.start(DNS_PORT, "*", apIP);
     server.on("/submit", HTTP_POST, handler);
   }

5. Main Loop
   void loop() {
     dnsServer.processNextRequest();
     server.handleClient();
   }

================================================================================
DEFENSE MECHANISMS
================================================================================

How to Protect Against Evil Twin Attacks:

| Defense                  | Description                                           |
|--------------------------|-------------------------------------------------------|
| Use VPN                  | Encrypts all traffic even on fake networks           |
| Check HTTPS              | Look for padlock in browser address bar              |
| Verify Certificate       | Don't ignore SSL certificate warnings                |
| Avoid Public Wi-Fi       | Use mobile hotspot when possible                     |
| Use WPA3                 | WPA3 has better protection against deauth attacks    |
| Enterprise WiFi          | Use 802.1X authentication (username + certificate)   |
| Network Monitoring       | Use tools like Wireshark to detect suspicious APs    |

Detection Signs:
- Multiple APs with the same name
- Network asks for credentials again immediately
- SSL certificate errors
- Unusual login page design
- Page URL doesn't match the service

================================================================================
LEGAL & ETHICAL NOTICE
================================================================================

WHAT IS LEGAL:
- Testing on your own network
- Testing with written permission from network owner
- Security research in controlled environments
- Educational demonstrations in cybersecurity classes

WHAT IS ILLEGAL:
- Stealing credentials from public Wi-Fi (Starbucks, airports, hotels)
- Attacking school/university networks without permission
- Harvesting passwords from neighbors
- Any unauthorized access to computer systems

Legal Consequences by Jurisdiction:

USA - Computer Fraud and Abuse Act (CFAA)
Penalty: Up to $500,000 + 20 years prison

UK - Computer Misuse Act 1990
Penalty: Up to 10 years prison

Sri Lanka - Computer Crimes Act No. 24 of 2007
Penalty: Fine up to Rs. 500,000 + 5 years prison

EU - GDPR + National Cyber Laws
Penalty: Fines up to 20 million Euros

================================================================================
TROUBLESHOOTING
================================================================================

Common Issues and Solutions:

Problem: Board not detected
Solution: Install correct USB driver or try different USB cable

Problem: Compilation error - 'success_page' not declared
Solution: Make sure success_page HTML block is defined before setup()

Problem: Fake network not showing
Solution: Check that WiFi.softAP() succeeded, verify ESP32 has power

Problem: Captive portal not redirecting
Solution: Verify DNS server is started: dnsServer.start(DNS_PORT, "*", apIP)

Problem: Credentials not saving
Solution: Check SPIFFS.begin(true) returns true, verify file write permissions

================================================================================
TECHNICAL SPECIFICATIONS
================================================================================

- Flash Memory Usage: ~975 KB (74% of 1310720 bytes)
- RAM Usage: ~45 KB (13% of 327680 bytes)
- Wi-Fi Standard: 802.11 b/g/n
- Operating Frequency: 2.4 GHz
- Default AP IP: 192.168.4.1
- DNS Port: 53
- HTTP Port: 80

================================================================================
CUSTOMIZATION
================================================================================

To change the Wi-Fi network name:
   const char* evil_ssid = "YOUR_NETWORK_NAME";

To add a password to the fake network:
   const char* ap_password = "your_password";

To modify the login page appearance:
   Edit the HTML/CSS code inside login_page variable

To change the redirect URL after login:
   Modify the meta refresh tag in success_page HTML

================================================================================
VERSION HISTORY
================================================================================

v1.0 - Initial release - Basic captive portal + admin panel
v1.1 - Added custom HTML/CSS interface with gradient backgrounds
v1.2 - Added SPIFFS storage and persistent credential saving
v1.3 - Added language selector (English/Sinhala/Tamil)
v1.4 - Added connected clients counter

================================================================================
DISCLAIMER (REPEATED FOR EMPHASIS)
================================================================================

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

YOU ARE SOLELY RESPONSIBLE FOR ANY DAMAGE OR LEGAL ACTION CAUSED BY USING THIS CODE. THE AUTHOR DOES NOT CONDONE ANY ILLEGAL ACTIVITIES.

================================================================================
CONTACT & SUPPORT
================================================================================

For educational questions or issues:
- Check the documentation thoroughly
- Search ESP32 forums at esp32.com
- Visit Arduino Stack Exchange at arduino.stackexchange.com
- Open an issue on GitHub (for bugs, not for hacking help)

================================================================================
ACKNOWLEDGMENTS
================================================================================

- Espressif for the ESP32 platform
- Arduino community for the libraries and IDE
- Security researchers who educate about Wi-Fi vulnerabilities
- Open source community for tools and inspiration

================================================================================
MADE FOR EDUCATIONAL PURPOSES ONLY
================================================================================

Remember: Security is everyone's responsibility. Learn to protect, not to attack.

Use this knowledge to:
- Secure your own networks
- Understand how attackers think
- Build better defense systems
- Educate others about cybersecurity
