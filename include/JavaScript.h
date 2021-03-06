// jsfiddle.net
const char JAVASCRIPT[] PROGMEM = R"=====(
//############ DO NOT CHANGE BELOW ###################
// alle GPIOs des ESP8266
const gpio = [  {port: 216, name:'D0'},
                {port: 205, name:'D1/SDA'},
                {port: 204, name:'D2/SCL'},
                {port: 200, name:'D3'},
                {port: 202, name:'D4'},
                {port: 214, name:'D5'},
                {port: 212, name:'D6'},
                {port: 213, name:'D7'},
                {port: 215, name:'D8'},
                {port: 201, name:'RX'},
                {port: 203, name:'TX'}
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
        _option.value = configuredPorts[i].port; 
        _option.text  = configuredPorts[i].name;
        if(objects[j].value == configuredPorts[i].port) { _option.selected = true;}
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
    _option.value = gpio[i].port; 
    if(gpio_disabled.indexOf(gpio[i].port)>=0) {_option.disabled = true;}
    if(value == (gpio[i].port)) { _option.selected = true;}
    _option.text  = gpio[i].name;
    _select.add( _option ); 
  }
  if (id.match(/^Alle.*/)) {
    // Alle PCF Ports in die Liste wenn ID match "Alle*"
    for ( k = 0; k < availablePorts.length; k++ ) {
      _option = document.createElement( 'option' );
      _option.value = _option.text = availablePorts[k];
      if(value == availablePorts[k]) { _option.selected = true;}
      _select.add( _option );
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
function createBiValPort(divID, num, sub) { 
  _div = document.createElement( 'div' ); 
  //_div.style.border = '1px solid #777';
  _div.id = divID +'_'+ num;
      
  _select = createPortSelectionList('AllePorts_' + divID + '_' + num, "pcfport_" + num + "_" + sub);
  _imp = document.createElement( 'input');
  _imp.type = 'number'; 
  _imp.id = _imp.name = 'imp_' + num + "_" + sub;
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
      td.appendChild(createBiValPort('PortA', num, 0));
    }
    if(document.getElementById('PortB_'+num)) {
      document.getElementById('PortB_'+num).style.display = 'inline';
    } else {
      td.appendChild(createBiValPort('PortB', num, 1));
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
  } 
  table = _td.parentNode.parentNode;
  validate_identifiers(table.id);
}

function delrow(btn) {
  table = btn.parentNode.parentNode.parentNode;
  if (btn.parentNode.parentNode.rowIndex != 1) {
    // erste Zeile ist das Template, darf nicht entfernt werden
    table.removeChild(btn.parentNode.parentNode);
    validate_identifiers(table.id);
  }
}

function addrow(tableID) { 
  _table = document.getElementById(tableID);
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
  validate_identifiers(tableID); // eigentlich obsolete
}

function validate_identifiers(tableID) {
  table = document.getElementById(tableID); 
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
  if(t && t.length>0) { t += '<br>Breche Speichervorgang ab. Es wurde nichts gespeichert!' }
  document.getElementById('ErrorText').innerHTML = t;
}

function checkRelationConfig(SubmitForm) {
  json = document.getElementById(SubmitForm).querySelectorAll("input[name='json']");
  result = JSON.parse(json[0].value);
  
  return true;
}

function checkVentilConfig(SubmitForm) {
  json = document.getElementById(SubmitForm).querySelectorAll("input[name='json']");
  result = JSON.parse(json[0].value);
  var topic = [], ports = [];
  for(var k in result) {
    // Quality Checks
    if(k.match(/^mqtttopic/)) { 
      if(topic.indexOf(result[k]) == -1) {
        topic.push(result[k]);
      } else {
        ShowError('Das MQTT Subtopic <b>' +result[k]+ '</b> wurde doppelt vergeben.');
        return false;
      }
    } else if(k.match(/^pcfport/)) { 
      if(ports.indexOf(result[k]) == -1) {
        ports.push(result[k]); 
      } else {
        ShowError('Der Port <b>' +result[k]+ '</b> wurde doppelt vergeben.');
        return false;
      }
    }
  }
  return true;
}

function onSubmit(DataForm, SubmitForm){
  // erstelle json String
  var formData = {};
  ShowError('');
  
  var elems = document.getElementById(DataForm).elements; 
  for(var i = 0; i < elems.length; i++){ 
    if(elems[i].name && elems[i].value) {
      if (elems[i].style.display == 'none') {continue;}
      if (elems[i].parentNode.tagName == 'DIV' && elems[i].parentNode.style.display == 'none') {continue;}
      if (elems[i].parentNode.parentNode.tagName == 'TR' && elems[i].parentNode.parentNode.style.display == 'none') {continue;}
      
      if (elems[i].type == "checkbox") {
        formData[elems[i].name] = (elems[i].checked==true?1:0);
      } else if (elems[i].id.match(/^Alle.*/) || elems[i].id.match(/^GpioPin.*/) || elems[i].type == "number") {
        formData[elems[i].name] = parseInt(elems[i].value);
      } else if (elems[i].type == "radio") {
        if (elems[i].checked==true) {formData[elems[i].name] = elems[i].value;}
      } else {
        formData[elems[i].name] = elems[i].value;
      }
    }
  } 
  formData["count"] = document.getElementById(DataForm).getElementsByClassName('editorDemoTable')[0].rows.length -1;
  json = document.getElementById(SubmitForm).querySelectorAll("input[name='json']");
  json[0].value = JSON.stringify(formData);
  
  return true;
}

/*******************************
blendet Zeilen der Tabelle aus
  a: Array of shown IDs 
  b: Array of hidden IDs 
*******************************/
function radioselection(a,b) {
  for(var i = 0; i < a.length; i++){
    if (document.getElementById(a[i])) {document.getElementById(a[i]).style.display = 'table-row';}
  }
  for(var j = 0; j < b.length; j++){
    if(document.getElementById(b[j])) {document.getElementById(b[j]).style.display = 'none';}
  }
}

)=====";
