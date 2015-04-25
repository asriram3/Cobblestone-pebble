Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);
var req = new XMLHttpRequest();
req.open('GET', 'http://coblestoner2.azurewebsites.net/', true);
req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    if(req.status == 200) {
      var response = JSON.parse(req.responseText);
			
			console.log(response.name);
			var name = response.name;
			var age = response.age.toString();
			var mar ="";
			if(response.married){
				mar = "Yes";
			}
			else{mar = "no";}
			var dict = {'name': name, 'age':age , 'married': mar};
      Pebble.sendAppMessage(dict);
    } else { console.log('Error'); }
  }
};
req.send(null);
