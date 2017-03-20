/*
 * Server.js
 *
 * The main portion of this project. Contains all the defined routes for express,
 * rules for the websockets, and rules for the MQTT broker.
 *
 * Refer to the portions surrounded by --- for points of interest
 */
var express   = require('express'),
	app       = express();
var pug       = require('pug');
var sockets   = require('socket.io');
var path      = require('path');
var mqttReq   = require('mqtt')
var client    = mqttReq.connect();

client.subscribe("1increment");
client.subscribe("2increment");
client.subscribe("3increment");
client.subscribe("4increment");
//
client.on("message", function(topic, payload) {
	console.log([topic, payload].join(": "));
	if(topic.search("increment") > 0) {
		var zoneId = topic.substring(0, topic.search("increment"));
		incrementPopulation(zoneId);
		// if(isOverCapacity(zoneId)){
		// 	var capacityTopic = zoneId + 'capacity';
		// 	client.publish(capacityTopic, "1");
		// }
		isOverCapacity(zoneId);
	}
});
//


var conf      = require(path.join(__dirname, 'config'));
var internals = require(path.join(__dirname, 'internals'));


//Vars we update as people enter areas
var population1 = 0, population2 = 0, population3 = 0, population4 = 0;

//Vars to keep track of max population at each Zone
var max1 = 2, max2 = 5, max3 = 3, max4 = 2;

// -- Setup the application
setupExpress();
setupSocket();

//Check if a Zone is over capacity and send a message to the capacity sensor if it is
function isOverCapacity(id){
	var tempPopulation = 0, tempMax = 1;

	switch(id){
		case '1':
			tempPopulation = population1;
			tempMax = max1;
			break;
		case '2':
			tempPopulation = population2;
			tempMax = max2;
			break;
		case '3':
			tempPopulation = population3;
			tempMax = max3;
			break;
		case '4':
			tempPopulation = population4;
			tempMax = max4;
			break;
		default:
			console.log('isOverCapacity: Invalid Zone ID');
			return false;
	}

	if(tempPopulation > tempMax) {
		console.log('Population in Zone ' + id + ' is ' + tempPopulation + ' which is greater than the max ' + tempMax);
		//if(isOverCapacity(client.id)){
			var capacityTopic = id + 'capacity';
			client.publish(capacityTopic, "1");
		//}
		return true;
	} else {
		return false;
	}
}

//Get the Population at a Zone.  Allows MQTT and REST to use the same interface
function getPopulation(id){
	var validInput = true;
	switch(id) {
		case '1':
			//isOverCapacity(id);
			return(population1);
			break;
		case '2':
			//isOverCapacity(id);
			return(population2);
			break;
		case '3':
			//isOverCapacity(id);
			return(population3);
			break;
		case '4':
			//isOverCapacity(id);
			return(population4);
			break;
		default:
			console.log('getPopulation: Invalid Zone ID');
			validInput = false;
			return 0;
	}

}

//Used to increment the Population at a zone, when a capacity sensor publishes its population.  Encapsulates the logic so that MQTT and REST can both increase the population from the same interface
function incrementPopulation(id){
	var validInput = true;
	switch(id){
		case '1':
			population1++;
			break;
		case '2':
			population2++;
			break;
		case '3':
			population3++;
			break;
		case '4':
			population4++;
			break;
		default:
			validInput = false;
	}

	if(validInput) {
		getPopulation(id);
	} else {
		console.log('incrementPopulation: Invalid Zone ID');
	}

}

// -- Socket Handler
// Here is where you should handle socket/mqtt events
// The mqtt object should allow you to interface with the MQTT broker through
// events. Refer to the documentation for more info
// -> https://github.com/mcollina/mosca/wiki/Mosca-basic-usage
// ----------------------------------------------------------------------------
function socket_handler(socket, mqtt) {
	// Called when a client connects
	mqtt.on('clientConnected', client => {
		socket.emit('debug', {
			type: 'CLIENT', msg: 'New client connected: ' + client.id
		});
		console.log('New client connected: ' + client.id);
	});

	// Called when a client disconnects
	mqtt.on('clientDisconnected', client => {
		socket.emit('debug', {
			type: 'CLIENT', msg: 'Client "' + client.id + '" has disconnected'
		});
		console.log('Client "' + client.id + '" has disconnected');
	});

	// Called when a client publishes data
	mqtt.on('published', (data, client) => {
		if (!client) return;

		socket.emit('debug', {
			type: 'PUBLISH',
			msg: 'Client "' + client.id + '" published "' + JSON.stringify(data) + '"'
		});


		// if(data.topic.search("increment") > 0) {
		// 	var incrementId = data.topic.substring(0, data.topic.search("increment"));
		// 	incrementPopulation(incrementId);
		// } else if (data.topic.search("capacity") > 0) {
		// 	console.log('Publishing over capacity');
		// 	client.publish()
		// }



		console.log('Client "' + client.id + '" published "' + JSON.stringify(data) + '"');


	});

	// Called when a client subscribes
	mqtt.on('subscribed', (topic, client) => {
		if (!client) return;

		socket.emit('debug', {
			type: 'SUBSCRIBE',
			msg: 'Client "' + client.id + '" subscribed to "' + topic + '"'
		});
		console.log('Client "' + client.id + '" subscribed to "' + topic + '"');
	});

	// Called when a client unsubscribes
	mqtt.on('unsubscribed', (topic, client) => {
		if (!client) return;

		socket.emit('debug', {
			type: 'SUBSCRIBE',
			msg: 'Client "' + client.id + '" unsubscribed from "' + topic + '"'
		});
		console.log('Client "' + client.id + '" unsubscribed from "' + topic + '"');
	});

	//mqtt.on('')
}
// ----------------------------------------------------------------------------


// Helper functions
function setupExpress() {
	app.set('view engine', 'pug'); // Set express to use pug for rendering HTML

	// Setup the 'public' folder to be statically accessable
	var publicDir = path.join(__dirname, 'public');
	app.use(express.static(publicDir));

	// Setup the paths (Insert any other needed paths here)
	// ------------------------------------------------------------------------
	// Home page
	app.get('/', (req, res) => {
		res.render('index', {title: 'MQTT Tracker'});
	});

	//This is where the RESTful API request handling will be

	//Increase the population at a beacon with a specified ID
	app.get('/incrementPopulation/:beaconID', (req, res) => {

		incrementPopulation(req.params.beaconID);
		isOverCapacity(req.params.beaconID);
		res.end( getPopulation(req.params.beaconID).toString() );
	});

	//Get the population of at a beacon with the specified ID
	app.get('/getPopulation/:beaconID', (req, res) => {
		var tempPopulation = getPopulation(req.params.beaconID)

		console.log('Zone ' + req.params.beaconID + ' has a population of ' + tempPopulation.toString());

		res.end( tempPopulation.toString() );
	});

	//End of RESTful API handling

	// Basic 404 Page
	app.use((req, res, next) => {
		var err = {
			stack: {},
			status: 404,
			message: "Error 404: Page Not Found '" + req.path + "'"
		};

		// Pass the error to the error handler below
		next(err);
	});

	// Error handler
	app.use((err, req, res, next) => {
		console.log("Error found: ", err);
		res.status(err.status || 500);

		res.render('error', {title: 'Error', error: err.message});
	});
	// ------------------------------------------------------------------------

	// Handle killing the server
	process.on('SIGINT', () => {
		internals.stop();
		process.kill(process.pid);
	});
}

function setupSocket() {
	var server = require('http').createServer(app);
	var io = sockets(server);

	// Setup the internals
	internals.start(mqtt => {
		io.on('connection', socket => {
			socket_handler(socket, mqtt)
		});
	});

	server.listen(conf.PORT, conf.HOST, () => {
		console.log("Listening on: " + conf.HOST + ":" + conf.PORT);
	});
}
