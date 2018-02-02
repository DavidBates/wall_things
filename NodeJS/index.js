import Twitter from 'twitter';
import storage from 'node-persist';

var Enum = require('enum');
const http = require('http');

const hostname = '0.0.0.0';
const port = process.env.PORT || 3000;
// This enum corresponds with the light addresses as wired. 
// Since the microcontroller is next to Z it's 0
var myEnum = new Enum({
  'A': 25,
  'B': 24,
  'C': 23,
  'D': 22,
  'E': 21,
  'F': 20,
  'G': 19,
  'H': 18,
  'I': 9,
  'J': 10,
  'K': 11,
  'L': 12,
  'M': 13,
  'N': 14,
  'O': 15,
  'P': 16,
  'Q': 17,
  'R': 8,
  'S': 7,
  'T': 6,
  'U': 5,
  'V': 4,
  'W': 3,
  'X': 2,
  'Y': 1,
  'Z': 0
});

const server = http.createServer((req, res) => {
  // Handle a request
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');
  var tweet = check_latest_tweet().then(
    function (data) {
      res.end(data);
    },
    function (err) {
      console.log(err);
    });

});

// These should not be stored in your repository. 
const tokens = {
  consumer_key: process.env.CKEY,
  consumer_secret: process.env.CSECRET,
  access_token: process.env.ATOKEN,
  access_token_key: process.env.ATKEY,
  access_token_secret: process.env.ATSECRET
};

// Setting up the twitter client
const client = new Twitter(tokens);
const params = {
  screen_name: 'wall_things'
};


function check_latest_tweet() {
  return new Promise(function (fulfill, reject) {
    // We store the already read tweets so we know if it's been displayed. 
    storage.initSync();
    client.get('statuses/mentions_timeline', params, (err, tweets) => {
      if (err) reject(err.toString());
      console.log(tweets.length);
      if (tweets != null) {
        // tweets = reverse(tweets);
        var level = 0
        tweets.forEach(function (tweet, idx, arr) {
          // if(idx == 0){
          //   level = arr.length;
          // }
          // idx = idx - level
          // console.log("IDX: " + idx + " level: " + level );
          if (tweet.lang.indexOf("en") > -1) {
            // This is the format of where to find the word. 
            if (tweet.text.indexOf("say:") > -1) {
              // If we've already displayed the latest tweet start displaying the last in the stack
              console.log(tweet.user.name + ": " + tweet.text);
              if (!storage.getItemSync(tweet.id_str) || idx <= 5) {
                
                storage.setItemSync(tweet.id_str, tweet.text);
                var matches = tweet.text.match(/.*(say: )(\w+).*/);
                var charArray = []
                for (var i = 0; i < matches[2].length; i++) {
                  var charIndex = myEnum.get(matches[2].charAt(i).toUpperCase()).value
                  charArray.push(charIndex);
                }
                var return_tweet = {
                  'user': tweet.user.name,
                  'tweet': tweet.text,
                  'chars': charArray
                }
                fulfill(JSON.stringify(return_tweet));
              }
            }
            // else{
            //   level = level - 1;
            // }
          }
          // else{
          //   level = level - 1
          // }
        })
      }
      // Default tweet if there are no tweets available. 
      fulfill(JSON.stringify({
        'user': "Will",
        'tweet': "RUN MOM RUN",
        'chars': [8, 5, 14]
      }));
    });

  });
}

function reverse(a) {
  var temp = [];
  var len = a.length;
  for (var i = (len - 1); i !== 0; i--) {
      temp.push(a[i]);
  }
  return temp;
}

var check_tweet = function () {

};
server.listen(port, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});