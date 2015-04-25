var id = Math.round(Math.random() * 255);
Pebble.addEventListener('ready', function(e) {
	console.log('JavaScript app ready and running!');
	//id=Pebble.getWatchToken();
});

Pebble.addEventListener('webviewclosed', function(e) {
	console.log('Application Closed');
	var req = new XMLHttpRequest();
	req.open('GET', 'http://coblestoner2.azurewebsites.net/leave/?id=' + id, true);
	req.send(null);
});


Pebble.addEventListener('appmessage', function(e) {
  console.log("it worked: " + e.payload.flag);
  
  if (e.payload.flag === 0) {
    var req1 = new XMLHttpRequest();
    console.log("I'm READY");
    req1.open('GET', 'http://coblestoner2.azurewebsites.net/ready/?id=' + id, true);
    req1.send(null);
 } else if (e.payload.flag === 1) {
    var req2 = new XMLHttpRequest();
    console.log('Application Closed');
    req2.open('GET', 'http://coblestoner2.azurewebsites.net/leave/?id=' + id, true);
    req2.send(null);
}
});

var do_once = true;
var req = new XMLHttpRequest();
req.open('GET', 'http://coblestoner2.azurewebsites.net/?id=' + id, true);
req.send(null);

function server_check() {
	//  console.log("Chekn");
	var req = new XMLHttpRequest();
	req.open('GET', 'http://coblestoner2.azurewebsites.net/gameInfo/?id=' + id, true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if (req.status == 200) {
				// console.log("pinged");
				//console.log(JSON.stringify(req));
				var response = JSON.parse(req.responseText);
				//console.log(response);

				var players = response.players.toString();
				var ready = response.ready.toString();
				var gameStatus = response.gameStatus.toString();
				//console.log("Players: "+players+" ready:"+ready+" game:"+gameStatus);
				var dict = { 'players': players, 'ready': ready , 'gameStatus': gameStatus };
				Pebble.sendAppMessage(dict);
			} else {
				console.log('Error');
			}
		}
	};
	req.send(null);
}

if (do_once) {
	do_once = false;
	console.log("Server Initialized");
	setInterval(function() {
		server_check();
    }, 300);
}
