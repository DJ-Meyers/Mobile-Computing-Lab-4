/*
 * Config.js
 *
 * Contains all of the configurable fields used by this project
 */
var path = require('path');

module.exports = {
	//Change ip to this later 192.168.0.101
	//localhost is 127.0.0.1
	HOST: '127.0.0.1',
	PORT: '8000',

	// Variables used internally
	MONGO_VERSION: '3.2.8',
	MONGO_PORT   : 27017,
	MONGO_DIR    : path.join(__dirname, '.mongo'),

	MOSCA_PORT   : 1883,

};
