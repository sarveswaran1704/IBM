fetch('/check_flood', {
  method: 'POST',
  body: new URLSearchParams({ 'pinCode': pinCode }),
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded',
  },
})
