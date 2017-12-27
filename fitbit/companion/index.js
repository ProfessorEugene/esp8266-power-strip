import { peerSocket, MessageSocket } from "messaging";
const STRIP_URL = "http://strip.local/";
const TIMEOUT_MS = 1000;
const ATTEMPTS = 3;

const req = (url) => new Promise((resolve, reject) => {
  let attempt = 0;
  const tryFetch = () => {
    attempt = attempt + 1;
    console.log('req: attempt ' + attempt + ' to fetch ' + url);
    if (attempt > ATTEMPTS) {
      console.log('req: timed out after ' + attempt + ' attempts of ' + TIMEOUT_MS +'ms');
      reject(new Error('Timed out'));
    } else {
      let retryTimeout = setTimeout(() => tryFetch(), TIMEOUT_MS);
      fetch(url)
        .then((response) => {
          console.log('req: raw response: ' + response);
          return response;
        })
        .then((response) => response.json())
        .then((jsonData) => {
          console.log('req: json response: ' + JSON.stringify(jsonData));
          clearTimeout(retryTimeout);
          resolve(jsonData);
        })
        .catch((err) => {
          console.log('req: fetch error: ' + err);
          clearTimeout(retryTimeout);
          reject(err)
        });
    }
  };
  tryFetch();
});

const getState = () => req(STRIP_URL + "get");

const setState = (state) => req(STRIP_URL + "set?s=" + state);

const pushState = (state) => {
  if (peerSocket.readyState === peerSocket.OPEN) {
    peerSocket.send({state: state});
    console.log('pushed state ' + state);
  } else {
    console.log('Error: Connection is not open');
  }
};

peerSocket.onopen = (evt) => {
  console.log('Connection to device open');
};

peerSocket.onclose = (evt) => {
  console.log('Connection to device closed');
};

/* handle messages from device */
peerSocket.onmessage = (evt) => {
  console.log('Recieved message from device');
  if (evt.data) {
    if('get' === evt.data.command){
      console.log('Obtaining state');
      getState()
        .then((json) => pushState(json.status))
        .catch((err) => {
          console.log('Error fetching strip state: ' + err.message);
        });
    } else if('set' === evt.data.command){
      const newState = parseInt(evt.data.state);
      console.log('Setting state to ' + newState);
      setState(newState)
        .then((json) => pushState(json.status))
        .catch((err) => {
          console.log('Error setting strip state: ' + err.message);
        });
    }
  }
}

/* handle errors */
peerSocket.onerror = (err) => {
  console.log('Connection error: ' + err.code + ' - ' + err.message);
};

console.log('Companion started');