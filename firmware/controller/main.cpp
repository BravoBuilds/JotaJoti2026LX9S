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
<!doctype html><html lang="en"><head><meta name="viewport" content="width=device-width,initial-scale=1"><meta name="theme-color" content="#08233b">
<title>ScoutLink Relay</title><style>
*{box-sizing:border-box}body{margin:0;background:#071827;color:#f4f8fc;font:16px/1.5 system-ui,sans-serif}
main{max-width:800px;margin:auto;padding:18px}.tag{color:#ffd166;font-weight:800;letter-spacing:.1em;font-size:.8rem}
h1{font-size:clamp(2rem,6vw,3rem);line-height:1.1;margin:8px 0}h2{font-size:1.15rem;margin:0 0 12px}
.muted{color:#afc4d4;font-size:.92rem}.grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:12px}
.card{background:#102d46;border:1px solid #28516d;border-radius:17px;padding:16px;margin:12px 0}
.big{font-size:1.8rem;font-weight:850}.label{font-size:.82rem;color:#afc4d4}
.gold{border-color:#78612a;background:#1b3044}.kind{color:#ffd166;font-size:.78rem;font-weight:800}
button{font:inherit;font-weight:800;border:0;border-radius:11px;padding:12px 14px;background:#ffd166;color:#102239;min-height:44px;cursor:pointer}
@media(max-width:460px){main{padding:14px}.grid{grid-template-columns:1fr}.card{padding:14px}}
</style></head><body><main>
<div class="tag">JOTA-JOTI • SCOUTLINK RELAY</div><h1>Scout camp dashboard</h1>
<p class="muted">Served directly by your Base Station ESP32. No LCD, cloud account or internet required.</p>
<div class="grid"><section class="card"><div class="big" id="events">0</div><div class="label">Field-node events</div></section>
<section class="card"><div class="big" id="status">WAITING</div><div class="label">Field Node status</div></section></div>
<section class="card gold"><div class="kind">CURRENT SCOUT CHALLENGE</div><h2 id="challenge">Loading…</h2><button onclick="nextChallenge()">Next challenge →</button></section>
<section class="card"><h2>Last field activity</h2><p id="event" class="muted">Waiting for the Field Node.</p><p class="muted">Connect your phone to Wi-Fi <b>JOTA-JOTI-2026</b> and open <b>http://192.168.4.1</b>.</p></section>
<script>
async function refresh(){try{const r=await fetch('/api/state');const s=await r.json();document.getElementById('status').textContent=s.online?'ONLINE':'WAITING';document.getElementById('events').textContent=s.events;document.getElementById('event').textContent=s.lastEvent;document.getElementById('challenge').textContent=s.challenge}catch(e){document.getElementById('status').textContent='RECONNECT'}}
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
