const char JAVASCRIPT[] PROGMEM = R"=====(
//############ DO NOT CHANGE BELOW ###################
// alle GPIOs des ESP8266
const gpio = [  {port: 16, name:'D0'},
                {port:  5, name:'D1'},
                {port:  4, name:'D2/SDA'},
                {port:  0, name:'D3/SCL'},
                {port:  2, name:'D4'},
                {port: 14, name:'D5'},
                {port: 12, name:'D6'},
                {port: 13, name:'D7'},
                {port: 15, name:'D8'},
                {port:  1, name:'RX'},
                {port:  3, name:'TX'}
             ];

window.addEventListener('load', init, false);
function init() {
  SetConfiguredPorts();
  SetAvailablePorts();
}

function SetConfiguredPorts() {
  var _parent, _select, _option, i, j, k;
  var objects = document.querySelectorAll('input[type=number][id^=ConfiguredPorts]');
  for( j=0; j< objects.length; j++) {
    _parent = objects[j].parentNode;
    _select = document.createElement('select');
    _select.id = objects[j].id;
    _select.name = objects[j].name;
    for ( i = 0; i < configuredPorts.length; i += 1 ) {
        _option = document.createElement( 'option' );
        _option.value = configuredPorts[i]['port']; 
        _option.text  = configuredPorts[i]['name'];
        if(objects[j].value == configuredPorts[i]['port']) { _option.selected = true;}
        _select.add( _option ); 
    }
    _parent.removeChild( objects[j] );
    _parent.appendChild( _select );
  }
}

function SetAvailablePorts() {
  var _parent, _select, _option, i, j, k;
  var objects = document.querySelectorAll('input[type=number][id^=AllePorts], input[type=number][id^=GpioPin]');
  for( j=0; j< objects.length; j++) {
    _parent = objects[j].parentNode;
    _select = createPortSelectionList(objects[j].id, objects[j].name, objects[j].value);
    _parent.removeChild( objects[j] );
    _parent.appendChild( _select );
  }
}

function createPortSelectionList(id, name, value) {
  _select = document.createElement('select');
  _select.id = id;
  _select.name = name;
  for ( i = 0; i < gpio.length; i += 1 ) {
    // alle GPIO Pins in die Liste
    _option = document.createElement( 'option' );
    _option.value = gpio[i]['port']+200; 
    if(gpio_disabled.indexOf(gpio[i]['port'])>=0) {_option.disabled = true;}
    if(value == (gpio[i]['port']+200)) { _option.selected = true;}
    _option.text  = gpio[i]['name'];
    _select.add( _option ); 
  }
  if (id.match(/^Alle.*/)) {
    // Alle PCF Ports in die Liste wenn ID match "Alle*"
    for ( k = 0; k < pcf_found.length; k++ ) {
      for ( i = 0; i < 8; i += 1 ) {
        _option = document.createElement( 'option' );
        _option.value = _option.text = pcf_found[k]+i;
        if(value == pcf_found[k]+i) { _option.selected = true;}
        _select.add( _option );
      }
    }
  }
  return _select;
}

function createNormalPort(divID, num) {
  _div = document.createElement( 'div' ); 
  //_div.style.border = '1px solid #777';
  _div.id = divID +'_'+ num;
  _select = createPortSelectionList('AllePorts_'+divID, "pcfport_" + num + "_0");
  _div.appendChild(_select);
  
  return _div;
}

// td: td-object, num: number, visiblility: bool
function showNormal(td, num, visiblility) {
  if(visiblility) {
    if(document.getElementById('Port_'+num)) {
      document.getElementById('Port_'+num).style.display = 'inline';
    } else {
      td.appendChild(createNormalPort('Port', num));
    }
  } else {
    if(document.getElementById('Port_'+num)) {
      document.getElementById('Port_'+num).style.display = 'none';
    }
  }
}

// id: id des DIV Containers (PortA/PortB), Num: laufende RowNumber
function createBiValPort(divID, num) { 
  _div = document.createElement( 'div' ); 
  //_div.style.border = '1px solid #777';
  _div.id = divID +'_'+ num;
      
  _select = createPortSelectionList('AllePorts_'+divID, "pcfport_" + num + "_1");
  _imp = document.createElement( 'input');
  _imp.type = 'number'; 
  _imp.id = _imp.name = 'imp_'+num;
  _imp.value = _imp.min = '10';
  _imp.max = '999'
  _label1 = document.createElement( 'label');
  _label1.innerHTML ='for';
  _label2 = document.createElement( 'label');
  _label2.innerHTML ='ms';
   
  _div.appendChild(_select);
  _div.appendChild(_label1);
  _div.appendChild(_imp);
  _div.appendChild(_label2);
  
  return _div;
}

// td: td-object, num: number, visiblility: bool
function showBiVal(td, num, visiblility) { 
  if(visiblility) {
    if(document.getElementById('PortA_'+num)) {
      document.getElementById('PortA_'+num).style.display = 'inline';
    } else {
      td.appendChild(createBiValPort('PortA', num));
    }
    if(document.getElementById('PortB_'+num)) {
      document.getElementById('PortB_'+num).style.display = 'inline';
    } else {
      td.appendChild(createBiValPort('PortB', num));
    }
  } else {
    if(document.getElementById('PortA_'+num)) {
      document.getElementById('PortA_'+num).style.display = 'none';
    }
    if(document.getElementById('PortB_'+num)) {
      document.getElementById('PortB_'+num).style.display = 'none';
    }
  }
}

function chg_type(id) {
  num = id.replace(/^type_(\d+).*/g, "$1");
  val = document.getElementById(id).value;
  _td  = document.getElementById('tdport_'+num);
  if (val == 'b') {
    // Typ "Bistabil"
    showBiVal(_td, num, true);
    showNormal(_td, num, false);
  } else if (val == 'n'){
    // Typ "normal"
    showBiVal(_td, num, false);
    showNormal(_td, num, true); 
  } else if (val == 'v') {
    // Typ "virtuell"
    showBiVal(_td, num, false);
    showNormal(_td, num, false);
  }
  validate_identifiers();
}

function delrow(btn) {
  table = btn.parentNode.parentNode.parentNode;
  if (btn.parentNode.parentNode.rowIndex != 1) {
    // erste Zeile ist das Template, darf nicht entfernt werden
    table.removeChild(btn.parentNode.parentNode);
    validate_identifiers();
  }
}

function addrow() { 
  _table = document.getElementById('maintable');
  _table.rows[1].style.display = '';
  new_row = _table.rows[1].cloneNode(true);
  num = _table.rows.length;
  new_row.cells[0].innerHTML = num; 
  objects = new_row.querySelectorAll('label, input, select, div, td');
  for( j=0; j< objects.length; j++) {
    if (objects[j].name) {objects[j].name = objects[j].name.replace(/(\d+)/, num);}
    if (objects[j].id) {objects[j].id = objects[j].id.replace(/(\d+)/, num);}
    if (objects[j].htmlFor) {objects[j].htmlFor = objects[j].htmlFor.replace(/(\d+)/, num);}
  }
  _table.appendChild(new_row);
  validate_identifiers(); // eigentlich obsolete
}

function validate_identifiers() {
  table = document.getElementById('maintable');
  for( i=1; i< table.rows.length; i++) { 
    row = table.rows[i];
    row.cells[0].innerHTML = i; 
    objects = row.querySelectorAll('label, input, select, div, td');
    for( j=0; j< objects.length; j++) {
      if (objects[j].name) {objects[j].name = objects[j].name.replace(/(\d+)/, i-1);}
      if (objects[j].id) {objects[j].id = objects[j].id.replace(/(\d+)/, i-1);}
      if (objects[j].htmlFor) {objects[j].htmlFor = objects[j].htmlFor.replace(/(\d+)/, i-1);}
    }
  }
}

function ShowError(t){
  if(t) { t += '<br>Breche Speichervorgang ab. Es wurde nichts gespeichert!' }
  document.getElementById('ErrorText').innerHTML = t;
}

function onSubmit(){
  // erstelle json String
  var formData = {};
  var topic = [], ports = [];
  ShowError('');
  
  var elems = document.getElementById('submitForm').elements; 
  for(var i = 0; i < elems.length; i++){ 
    if(elems[i].name && elems[i].value) {
      if (elems[i].style.display == 'none') {continue;}
      if (elems[i].parentNode.tagName == 'DIV' && elems[i].parentNode.style.display == 'none') {continue;}
      if (elems[i].parentNode.parentNode.tagName == 'DIV' && elems[i].parentNode.parentNode.style.display == 'none') {continue;}
      
      if (elems[i].type == "checkbox") {
        formData[elems[i].name] = (elems[i].checked==true?1:0);
      } else if (elems[i].id.match(/^Alle.*/) || elems[i].type == "number") {
        formData[elems[i].name] = parseInt(elems[i].value);
      } else if (elems[i].type == "radio") {
        if (elems[i].checked==true) {formData[elems[i].name] = elems[i].value;}
      } else {
        formData[elems[i].name] = elems[i].value;
      }
      
      if(elems[i].name.match(/^mqtttopic/)) {
        if(topic.indexOf(elems[i].value) == -1) {
          topic.push(elems[i].value);
        } else {
          ShowError('Das MQTT Subtopic <b>' +elems[i].value+ '</b> wurde doppelt vergeben.');
          return false;
        }
      } else if(elems[i].id.match(/^Alle/)) { 
        if(ports.indexOf(elems[i].value) == -1) {
          ports.push(elems[i].value); 
        } else {
          ShowError('Der Port <b>' +elems[i].options[elems[i].selectedIndex].text+ '</b> wurde doppelt vergeben.');
          return false;
        }
      }
    }
  } 
  formData["count"] = document.getElementById('maintable').rows.length -1;
  document.getElementById("json").value = JSON.stringify(formData);
}
)=====";
