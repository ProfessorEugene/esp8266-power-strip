import document from "document";
import { peerSocket, MessageSocket } from "messaging";
// import * as messaging from "messaging";
//const messaging = {peerSocket:{readyState:'CLOSED', OPEN:1}};

const FILL_GREEN = 'fb-green';
const FILL_RED = 'fb-red';

/* configure a state */
let state = 0x0F;
/* pull plug elements */
const plugElements = document.getElementsByClassName('plug');
const statusElement = document.getElementById('status');
/* function to push state */
const pushState = () => {
  if (peerSocket.readyState === peerSocket.OPEN) {
    peerSocket.send({
      command: 'set',
      state: state,
    });
    log('Pushed state ' + state);
  }
};
/* function to log message to display */
const log = (msg) => statusElement.text = msg;

peerSocket.onbufferedamountdecrease = () => {
  /* do nothing */
}

/* as soon as a socket is available, try to get state! */
peerSocket.onopen = () => {
  log('Connected peer');
  peerSocket.send({
    command: 'get',
  });
  log('Sent update req');
};

peerSocket.onclose = () => {
  log('Disconnected peer');
}

/* listen for state updates */
peerSocket.onmessage = (evt) => {
  log('Got update resp');
  if (evt.data) {
    console.log(JSON.stringify(evt));
    /* update state variable */
    state = parseInt(evt.data.state);
    /* update each button background! */
    plugElements.forEach((pe, pi) => {
      if((state >> pi) & 0x1 === 0x1){
         pe.status = 1;
      }else {
        pe.status = 0;
      }
      pe.style.fill = (pe.status === 0) ? FILL_GREEN : FILL_RED;
    });
  }
};

/* handle errors */
peerSocket.onerror = (err) => {
  log('Peer comms error');
};

/* configure UI! */
plugElements.forEach((pe, pi) => {
    pe.status = 0;
    pe.onactivate = (evt) => {
      /* bump status */
      pe.status = (pe.status === 0) ? 1 : 0;
      /* change fill */
      pe.style.fill = (pe.status === 0) ? FILL_GREEN : FILL_RED;
      /* update state */
      state = state ^ (0x01 << pi);
      /* push state! */
      pushState();
    };
});
