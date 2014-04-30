
var current_token = "0000";
var TEMPO = 1;

var tmr_fala;


var twitter = require('ntwitter');
var http = require('http');
var fs = require('fs');

var sys = require('sys')
var exec = require('child_process').exec;


var config = require('./config.json');

var twit = new twitter(config.twitter);


console.log("OK");

/*


	"filter": ["#forumbr", "#SurveyNoters", "#GirlsFamousLine", "#NoongBataPaAko"],

	"keywords": {
		"green": ["amarelo", "surveynoters"],
		"yellow": ["verde", "girlsfamousline"],
		"red": ["vermelho", "noongbatapaako"]
	}	





	"filter": ["#forumbr"],

	"keywords": {
		"green": ["verde", "green"],
		"yellow": ["amarelo", "yellow"],
		"red": ["vermelho", "red"]
	}	




	"filter": ["#forumbr"],

	"keywords": {
		"green": ["amarelo", "notadez"],
		"yellow": ["verde", "ajuda"],
		"red": ["vermelho", "oops"]
	}	




*/



var user_votes = {};
var votes_by_color = {
	"green": {
		"votes": 0,
		"percent": 0
	},
	"yellow": {
		"votes": 0,
		"percent": 0
	},
	"red": {
		"votes": 0,
		"percent": 0
	},

	"total": 0
}


// twit
// 	.verifyCredentials(function (err, data) {
// 		console.log(data);
// 	})
// 	.updateStatus('Testando tweet :D /' + twitter.VERSION,
// 		function (err, data) {
// 			console.log(data);
// 		}
// 	);




var SerialPort = require("serialport").SerialPort;
var serialPort = new SerialPort(config.serialPort, {
  baudrate: 9600
});

serialPort.on("open", function () {
	console.log('Serial opened!');

	serialPort.on('data', function(data) {
		//console.log('SERIAL: ' + data);
	});

	setTimeout(function(){
		serialPort.write("blue\n", function(err, results) {
			console.log('err ' + err);
			console.log('results ' + results);
		});		
	}, 1000);

});



var all_keys = [];
for (var color in config.keywords) {
	all_keys = all_keys.concat(config.keywords[color]);
}
//var all_keys = [].concat(config.keywords.green).concat(config.keywords.yellow).concat(config.keywords.red);



var get_variation = function(current, last) {
	if (current > last) {
		return " ^";
	} else if (current < last) {
		return " v";
	} else {
		return "  ";
	}
}


var last_resume = "";

var recalculate_percents = function() {
	var resume = "";
	for (var idx in votes_by_color) {
		if (idx !== "total") {
			var last_percent = votes_by_color[idx].percent;
			votes_by_color[idx].percent = Math.floor(votes_by_color[idx].votes / votes_by_color["total"] * 100);
			resume += zeroPad(votes_by_color[idx].percent, 3) + get_variation(votes_by_color[idx].percent, last_percent) + "\t";
			serialPort.write(zeroPad(votes_by_color[idx].percent, 3));
		}
	}

	serialPort.write("\n");

	if (true || resume != last_resume) {
		//console.log(votes_by_color.total + ": \t" + resume);
		last_resume = resume;
	}



}



var has_vote = function(tweet) {
	// procurar pela chave principal e pelos tipos de votos

	if (tweet.entities && tweet.entities.hashtags) {
		for (var idx_hash in tweet.entities.hashtags) {
			var hash = tweet.entities.hashtags[idx_hash];
			var idx = all_keys.indexOf(hash.text.toLowerCase())
			if (idx > -1)
			{
				return true;
			}

		}
	}
	return false;
}

var get_vote_type = function(tweet) {
	if (tweet.entities && tweet.entities.hashtags) {

		for (var idx_hash in tweet.entities.hashtags) {

			var hash = tweet.entities.hashtags[idx_hash];
			for (var color in config.keywords) {
				if ( config.keywords[color].indexOf(hash.text.toLowerCase()) > -1 ) {
					return color;
				}
			}
		}

		return null;
	}

}




var compute_vote = function(tweet) {
	var vote_type = get_vote_type(tweet);

	if (user_votes[tweet.user.screen_name]) {
		//novo antigo
		var previous_vote = user_votes[tweet.user.screen_name];
		votes_by_color[previous_vote].votes--;
	}

	votes_by_color[vote_type].votes++;
	user_votes[tweet.user.screen_name] = vote_type;
	votes_by_color["total"] = Object.keys(user_votes).length;

	recalculate_percents();
}



var get_stream = function() {


	twit.stream("statuses/filter", { "track": config.filter.join(", ") },

		function(stream) {

			stream.on('data', function(tweet) {
				//console.log("new tweet");

				if (has_vote(tweet)) {
					console.log(tweet.user.screen_name + ": " + tweet.text);
					console.log(tweet.text);
					console.log("********************************");

					return compute_vote(tweet);

				} else {

					//console.log(tweet.user.id);
					//console.log(tweet.user.screen_name + ": " + tweet.text);
					//console.log(tweet.retweeted_status != null); //eh um RT de algum outro user (contar sim)
					//console.log(tweet.entities.hashtags);
					//console.log();

				}


			});


			stream.on('end', function (response) {
				// Handle a disconnection
				console.log("END");
				get_stream();
			});

			stream.on('destroy', function (response) {
				// Handle a 'silent' disconnection from Twitter, no end/error event fired
				console.log("DESTROY");
			});

		});

}

get_stream();



function zeroPad(num, places) {
  var zero = places - num.toString().length + 1;
  return Array(+(zero > 0 && zero)).join("0") + num;
}
