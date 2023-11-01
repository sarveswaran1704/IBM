document.addEventListener('DOMContentLoaded', function () {
  const pinInput = document.getElementById('pinInput');
  const submitButton = document.getElementById('submitButton');
  const alertsDiv = document.getElementById('alerts');

  submitButton.addEventListener('click', function () {
    const pinCode = pinInput.value;

    // Request notification permission when the user clicks the "Submit" button.
    requestNotificationPermission();

    // Make an HTTP request to your Beeceptor endpoint.
    fetch('https://floodmonitoring.free.beeceptor.com.')
      .then(response => response.json())
      .then(data => {
        // Replace this logic with how you want to process the received data.
        // For this example, we simulate a high alert if the received data is high.
        if (pinCode === '607004') {
          if (data && data.value > 500) {
            sendHighAlertNotification();
          }
          showAlert('No flood alerts for PIN code 12345.');
        } else {
          showAlert('Flood alert for PIN code ' + pinCode + '!');
        }
      })
      .catch(error => {
        console.error('Error:', error);
      });
  });

  function showAlert(message) {
    const alertElement = document.createElement('p');
    alertElement.textContent = message;
    alertsDiv.appendChild(alertElement);
  }

  function requestNotificationPermission() {
    if ('Notification' in window) {
      Notification.requestPermission().then(function (permission) {
        if (permission === 'granted') {
          console.log('Notification permission granted.');
        } else {
          console.log('Notification permission denied.');
        }
      });
    }
  }

  function sendHighAlertNotification() {
    if ('Notification' in window) {
      if (Notification.permission === 'granted') {
        const notification = new Notification('High Flood Alert', {
          body: 'High water level detected. Take necessary precautions.',
          icon: 'notification-icon.png' // You can provide an icon image.
        });
      }
    }
  }
});
