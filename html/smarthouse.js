var ws = new WebSocket('ws://127.0.0.1:9000/','dumb-increment-protocol');

var name_disp;

ws.onmessage = function(event) {
    var answer = event.data.split(" ");
    var idx = "value_" + answer[0];
    document.getElementById(idx).innerHTML = answer[1];
}

function setDigital(id){
  var btn = document.getElementById(id);
  var value = btn.innerHTML;
  if (value == 'ON'){
    btn.innerHTML = 'OFF';
    var command = "set_channel_value_web" + " " + name_disp + " " + id + " " + 0;
    ws.send(command);

    console.log("dati inviati");
  }else{
    btn.innerHTML = 'ON';
    var command = "set_channel_value_web" + " " + name_disp + " " + id + " " + 1;
    ws.send(command);

    console.log("dati inviati");
  }
}

function setPwm(id){
  var btn = document.getElementById(id);
  var value = btn.value;
  if (value > 255 || value < 0){
    console.log("Valore deve essere compreso tra 0 e 255");
    btn.value = 0;
  }else{

    var valore_id = "value_" + id;
    var valore_pwm = document.getElementById(valore_id);
    valore_pwm.innerHTML = value;

    var command = "set_channel_value_web" + " " + name_disp + " " + id + " " + value;
    ws.send(command);
  }
}

function setName(id){
  var nome = document.getElementById(id);
  var name_device = nome.value;
  var command = "set_name " + name_device;
  name_disp = name_device;
  nome.value = "";

  ws.send(command);

}

function getDigital(id){
  var command = "get_channel_value_web" + " " + name_disp + " " + id;
  ws.send(command);
}

function getAnalog(id){
  var command = "get_adc_channel_value_web" + " " + name_disp + " " + id;
  ws.send(command);
}
