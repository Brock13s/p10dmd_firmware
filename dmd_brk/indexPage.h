#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

const char* indexContent = R"(
<!DOCTYPE html>
<html>
<head>

  <title>Brocks DMD Pixel art drawer</title>
  <style>
    html,body {
      display: flex;
      flex-direction: column;
      background-color: black;
      width: 100%;
      

      align-items: center;
      height: 100vh;
      margin: 0;
    }
    
    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-bottom: 10px;
    }
    
    .grid {
      display: grid;
      grid-template-columns: repeat(32, 20px);
      grid-template-rows: repeat(16, 20px);
      max-width: 640px;
      width: 100%;
    }
    
    .button {
      width: 20px;
      height: 20px;
      margin-top: 10px;
      background-color: black;
      border: 2px solid #3EB489;
    }

    .button:hover, .button.green:hover{
      background-color: #3EB489;
      transition: 0.2s;
    }
    
    .button.green {
      background-color: #ff0000;
    }
    
    .clear-button {
      margin-top: 30px;
      margin-bottom: 20px;
      padding: 10px 20px;
      border: 2px solid #3EB489;
      background-color: black;
      color: #3EB489;
      font-size: 16px;
      cursor: pointer;
    }

    .settings-button{
      margin-bottom: 20px;
      padding: 10px 20px;
      border: 2px solid #3EB489;
      background-color: black;
      color: #3EB489;
      font-size: 16px;
      cursor: pointer;
    }

    .clear-button:hover, .settings-button:hover{
      background-color: #3EB489;
      color: black;
      transition: 0.5s;
    }

    .header {
      font-size: 24px;
      margin-bottom: 20px;
      margin-top: 30px;
      color: #3EB489;
    }

    hr {
    border: 1px solid #3EB489;
    width: 100%;
    box-sizing: border-box;
  }

  /* Style for the text input field */
  input[type="text"] {
    border: 2px solid #3EB489;
    background-color: black;
    color: #3EB489;
    padding: 5px 10px;
    font-size: 16px;
    margin-right: 10px;
    margin-bottom: 10px;
  }

  label{
    color:#3EB489;
  }

  /* Add hover effect for the send button */
  #send-button {
    border: 2px solid #3EB489;
    padding: 5px 10px;
    background-color: black;
    color: #3EB489;
    font-size: 16px;
    transition: 0.5s;
  }
  #send-button:hover {
    cursor: pointer;
    background-color: #3EB489;
    color: black;
    transition: 0.5s;
  }
  </style>
</head>
<body>
  <h1 class="header">Brocks DMD pixel art drawer</h1>
  <hr>


    <form class="grid" action="/" method="post">
      <!-- Generate buttons using JavaScript -->
    </form>
  
    <button class="clear-button">Clear All</button>
    <a href="/settings"><button class="settings-button">Settings</button></a>
    <hr>
<div class="border">
  <label for="text-input">Enter DMD text:</label>
  <input type="text" id="text-input">
  <button id="send-button">Send</button>
</div>
<hr>

  <script>
    document.addEventListener('DOMContentLoaded', function() {
      function updateButtonStates(){
        fetch('/getButtonStates')
        .then(response => response.text())
        .then(data => {
          for(let i = 0; i<data.length; i++){
            buttons[i].classList.toggle('green', data[i] === '1');
          }
        })
        .catch(error => console.error('Error fetching button states: ', error));
      }
      updateButtonStates();
      const grid = document.querySelector('.grid');
      const buttons = [];

      // Generate buttons and add event listeners
      for (let i = 0; i < 32; i++) {
        for (let j = 0; j < 16; j++) {
          const button = document.createElement('div');
          button.className = 'button';
          button.isGreen = false;
          buttons.push(button);
          grid.appendChild(button);

          // Add click event listener
          button.addEventListener('mousedown', function(event) {
            button.classList.toggle('green');
            event.preventDefault()

            const buttonIndex = buttons.indexOf(button);
            const column = Math.floor(buttonIndex / 32);
            const row = (buttonIndex % 32);
            const state = button.classList.contains('green') ? 1 : 0;
            console.log(`Row ${row.toString()} Column ${column.toString()}`);

            const data = `row=${row}&column=${column}&state=${state}`;

fetch('/', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded'
  },
  body: data
});
          });


        

        
          button.addEventListener('contextmenu', function(event) {
            event.preventDefault();
          });
        }
      }

      const sendbtn = document.getElementById('send-button');
      sendbtn.addEventListener('click', function(){
          const textInput = document.getElementById('text-input');
          const textValue = textInput.value;
          console.log(textValue);
          if(textValue.trim() !== ''){
            const data = `text=${encodeURIComponent(textValue)}`;
            fetch('/text', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
              },
              body: data
            });
          }
      });
      

      // Add event listener to clear button
      const clearButton = document.querySelector('.clear-button');
      clearButton.addEventListener('click', function() {
        buttons.forEach(function(button) {
          button.classList.remove('green');
        });
        fetch('/clearall', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded'
  },
  body: 'clearall'
});
      });    // Add event listener to dropdown menu
      
    });

    
  </script>
</body>
</html>

)";

#endif
