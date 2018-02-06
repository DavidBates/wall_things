# Wall Things
The wall was introduced in season 1 of Stranger Things. It was a way for Will (the lost child) to communicate to his mother from the Upside Down. This came after his mother thought she was going crazy with flashing lights in the house. She finally figured out that her son was reaching out to her through the lights to attempt to communicate danger to her. 

## Why make a tweetable wall? 
Just like the show there was a supernatural connection going on. I thought that having twitter being the upside down and the wall being the physical manifestation of it kinda cool. Plus I always wanted to play with WS8211 (NeoPixels) and this offered the best of both worlds. I had seen others recreate the Stranger Things wall but none had connected it to twitter. 

## How it works
An ESP8266 board running the firmware connects to a nodeJS server about every ~6 seconds. This nodeJS server checks the mentions timeline for a tweet containing a particular pattern. If found it return the latest tweet in a specific JSON format. The ESP8266 then parses this tweet, displays it on the LCD and prepares the lights to spell out the text pattern it matched.

___
# Getting Started
### Setup you environment variables: 
`export CKEY="TwitterKey"`

`export CSECRET="TwitterSecret"`
`export ATOKEN="TwitterToken"`
`export ATKEY="Twitter-User-Key"`
`export ATSECRET="Twitter-User-Secret"`
### Run the dev nodeJS server
`cd NodeJS
yarn or npm install
npm run dev-start`
