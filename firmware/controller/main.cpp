#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "JOTA-JOTI-2026";
const char* AP_PASSWORD = "ScoutLink2026";
WebServer server(80);

struct FieldState {
  unsigned long lastSeen = 0;
  unsigned long events = 0;
  String lastEvent = "Waiting for field node...";
};
FieldState field;

const char* challenges[] = {
  "RADIO SCOUT: Tap a short Morse message with the Scout button.",
  "GREEN SCOUT: Name one way your camp can save electricity.",
  "WORLD SCOUT: Create a greeting for a Scout in another country.",
  "DIGITAL SCOUT: State one rule for staying safe online.",
  "TEAM SCOUT: Complete a challenge together and report your result."
};
const int challengeCount = sizeof(challenges) / sizeof(challenges[0]);
int challengeIndex = 0;

const char DASHBOARD[] PROGMEM = R"rawliteral(
<!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>ScoutLink Relay</title>
<style>
body{font-family:system-ui;background:#071b2a;color:#f5f7fa;margin:0}
main{max-width:900px;margin:auto;padding:24px}.card{background:#10344b;border:1px solid #2b607c;border-radius:16px;padding:18px;margin:14px 0}
h1{margin-bottom:4px}.badge{display:inline-block;padding:6px 10px;border-radius:999px;background:#f2c94c;color:#071b2a;font-weight:700}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:12px}
button{border:0;border-radius:10px;padding:12px 16px;font-weight:700;cursor:pointer}.muted{color:#b9ccd8}
</style></head><body><main>
<span class="badge">JOTA-JOTI 2026</span><h1>ScoutLink Relay</h1>
<p class="muted">Two ESP32s • one Scout camp • connected by curiosity</p>
<div class="grid">
<div class="card"><h3>Field Node</h3><div id="online">Checking...</div></div>
<div class="card"><h3>Events</h3><div id="events">0</div></div>
<div class="card"><h3>Last event</h3><div id="event">—</div></div>
</div>
<div class="card"><h2>Scout Challenge</h2><p id="challenge"></p><button onclick="nextChallenge()">New Challenge</button></div>
<div class="card"><h2>How it works</h2><pre>ESP32 #1 → Wi-Fi Access Point → ESP32 #2
Base Station       HTTP/JSON       Field Node</pre></div>
<script>
async function refresh(){const r=await fetch('/api/state');const s=await r.json();
document.getElementById('online').textContent=s.online?'ONLINE':'WAITING';
document.getElementById('events').textContent=s.events;
document.getElementById('event').textContent=s.lastEvent;
document.getElementById('challenge').textContent=s.challenge;}
async function nextChallenge(){await fetch('/api/next');refresh()}
refresh();setInterval(refresh,1500);
</script></main></body></html>
)rawliteral";

String jsonEscape(String value) {
  value.replace("\\", "\\\\");
  value.replace(""", "\\"");
  value.replace("\n", "\\n");
  value.replace("\r", "\\r");
  return value;
}

void handleRoot(){server.send(200,"text/html",DASHBOARD);}

void handleState(){
  bool online = field.lastSeen > 0 && (millis()-field.lastSeen) < 15000;
  String json="{"online":" + String(online?"true":"false");
  json+=","events":"+String(field.events);
  json+=","lastEvent":""+jsonEscape(field.lastEvent)+""";
  json+=","challenge":""+jsonEscape(String(challenges[challengeIndex]))+""}";
  server.send(200,"application/json",json);
}

void handleNext(){challengeIndex=(challengeIndex+1)%challengeCount;server.send(200,"application/json","{"ok":true}");}

void handleHeartbeat(){field.lastSeen=millis();server.send(200,"application/json","{"ok":true}");}

void handleEvent(){
  if(!server.hasArg("plain")){server.send(400,"application/json","{"error":"missing body"}");return;}
  field.lastSeen=millis();
  field.events++;
  field.lastEvent=server.arg("plain");
  server.send(200,"application/json","{"ok":true}");
}

void handleChallenge(){
  String json="{"index":"+String(challengeIndex)+","text":""+jsonEscape(String(challenges[challengeIndex]))+""}";
  server.send(200,"application/json",json);
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID,AP_PASSWORD);
  Serial.print("ScoutLink AP: ");Serial.println(WiFi.softAPIP());
  server.on("/",HTTP_GET,handleRoot);
  server.on("/api/state",HTTP_GET,handleState);
  server.on("/api/next",HTTP_GET,handleNext);
  server.on("/api/heartbeat",HTTP_GET,handleHeartbeat);
  server.on("/api/event",HTTP_POST,handleEvent);
  server.on("/api/challenge",HTTP_GET,handleChallenge);
  server.begin();
}

void loop(){server.handleClient();}
