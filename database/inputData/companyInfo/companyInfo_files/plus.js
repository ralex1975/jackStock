(function() {
	var closeFunctions = [];

	togglePluslager = function(linkObj, url, extraParams) {

        if (linkObj.parentNode === null) return;

		var trObj    = _findParent("tr", linkObj);
		var tableObj = _findParent("table", trObj);
		var resetId  = tableObj.id + trObj.id;

		// Ska vi visa eller dölja plusmenyn?
		var action = $(linkObj).hasClassName("showing") ? "hide" : "show";

		// Kolla om vi måste stänga en redan öppen plusmeny i optioner
		if (closeFunctions[resetId] && action == "show") {
			_runReset(resetId);
		}

		// 1. Markera raden i tabellen
		// colorTr(trObj.id);

		// 2. Markera plusknapp (vanlig plusmeny) eller optionsnamnet (kalkylatorn)
	        var buttons = linkObj.select('img');
		if (buttons.length == 1) {
			_changeKnapp(buttons[0], action);
		} else {
			_changeLink(linkObj, action);
		}

		// 3. Visa eller dölj plusmenyn
		if (action == "show") {
			_createPluslager(trObj, tableObj, url, extraParams);

			// Classen har ingen styling, används bara för att hålla reda på state
			$(linkObj).addClassName("showing");

			// Plockar tillfälligt bort eventuell class="last" från <tr>
			_swapClass(trObj, "last", "_last");

			/* raden nedan ger upphov till bugg 7025 /Kalle */
			// _setReset(resetId, linkObj);
		} else {
			_removePlusLayer(trObj, tableObj);

			$(linkObj).removeClassName("showing");

			// Lägger tillbaka eventuell class="last" på <tr>
			_swapClass(trObj, "_last", "last");

			_resetReset(resetId);
		}
	};

	closePluslager = function(closeLinkObj) {
		var trObj    = $(_findParent("tr", closeLinkObj)).previous();
		var tableObj = _findParent("table", trObj);
		var resetId  = tableObj.id + trObj.id;
		
		if (closeFunctions[resetId]) _runReset(resetId);
	};

	// Stäng en redan öppen plusmeny (i optioner)
	function _runReset(id) {
		reset = closeFunctions[id];
		_resetReset(id);
		reset();
	}

	// Göm en resetfunktion i DOM som kommer ihåg hur vi stänger oss själva
	function _setReset(id, linkObj) {
		closeFunctions[id] = function() { togglePluslager(linkObj); };
	}

	// Ta bort eventuell resetfunktion för den här raden
	function _resetReset(id) {
		closeFunctions[id] = null;
	}

	// Växla mellan plusknapp och minusknapp
	function _changeKnapp(imgObj, action) {
		if (action == "show") {
			imgObj.src = imgObj.src.replace("plus.gif", "minus.gif");
		} else {
			imgObj.src = imgObj.src.replace("minus.gif", "plus.gif");
		}
	}

	// Färga länken för optionskalkylatorn (som inte har någon plusknapp)
	function _changeLink(linkObj, action) {
		if (action == "show") {
			$(linkObj).addClassName("highlight");
		} else {
			$(linkObj).removeClassName("highlight");
		}
	}

	// Stega igenom parents uppåt till vi hittar ett element med önskad typ
	function _findParent(type, startObj) {
        if (startObj === null) return null;

		type = type.toUpperCase();

		var obj = startObj;
		while (obj.tagName != null && obj.tagName != type) {
			obj = obj.parentNode;
		}	

		return obj;
	}

	function _swapClass(trObj, fromClass, toClass) {
		if ($(trObj).hasClassName(fromClass)) {
			$(trObj).removeClassName(fromClass);
			$(trObj).addClassName(toClass);
		}
	}

	// Skapar ny <tr> och fyller med Ajax.Updater
	function _createPluslager(trObj, tableObj, url, extraParams) {
		var radid   = trObj.id;
		var innerId = "inner_" + trObj.id;
		var outerId = "outer_" + trObj.id;

		// Beräkna colspan som antalet <td> i raden som ska ha plusmeny
		var colspan = trObj.getElementsByTagName('td').length;

		// Slå ihop parametrar och defaultparametrar
		var params = $H({ plusid: innerId, tabellnamn: tableObj.id }).merge(extraParams);

		// Skapa ny <tr><td>div> direkt under nuvarande <tr>
		var newrow = tableObj.insertRow(trObj.rowIndex + 1);
		var cell = newrow.insertCell(0);

		newrow.setAttribute("id", "plus_" + tableObj.id + "_" + trObj.id);
	
		cell.className = "plusmeny";
		cell.colSpan   = colspan;
	
		cell.innerHTML = "<div class='switchcontent' id='" + outerId + "'><br /><br /><br /><br /><br /></div>";

		new Ajax.Updater(outerId, '/mux/ajax/' + url, { parameters: params, evalScripts: true });
	}

	// Plockar bort <tr> med pluslager från angiven tabell
	function _removePlusLayer(trObj, tableObj){
		var row = $("plus_" + tableObj.id + "_" + trObj.id);
		if (row) $(tableObj).deleteRow(row.rowIndex);
	}

})();

function visaFlerOrderdjup(plusid){
	var url = "/mux/ajax/marknaden/aktieinformation/plus/plusOrderdjup.html";
	var param = "plusid=" + plusid + "&multi=1";
	var div = 'orderdjup' + plusid;
	new Ajax.Updater(div, url, {parameters:param});
}

/* Lägger till ett papper i mina listor */
function laggTillIMinaListorPlus(listdiv, plusid, plusdiv){
	var lista = $(listdiv);
	var listid = lista.value;
	var listnamn = lista[lista.selectedIndex].text;
	// listnamn = listnamn.replace(/\?/,"\¤");
	var parsPlus ="plusid=" + plusid + "&listnamn=" + listnamn + "&listid=" + listid + "&plusdiv=" + plusdiv;
	var url = '/mux/ajax/marknaden/aktieinformation/plus/plusLaggTillLista.html';

	new Ajax.Updater(plusdiv, url, {parameters:parsPlus,evalScripts:true});
}

function skapaListaPlus(plusid, plusdiv){
	var listform = 'skapaListForm' + plusid;
	var listnamn = $(listform).listnamn.value;
	var parsPlus ="plusid=" + plusid + "&listnamn=" + listnamn + "&plusdiv=" + plusdiv;
	var url =  '/mux/ajax/marknaden/aktieinformation/plus/plusSkapaLista.html';

	new Ajax.Updater(plusdiv, url, {parameters:parsPlus, asynchronous:'false', evalScripts:true});
}

function myListInstruments(action, listid, instruments, successfunc, failfunc){

	var counter = instruments.length;
	var error = false;

	var returnhandler = function(transport) {
		var value = transport.responseText.strip();
		counter--;
		if(value<0){
			error = true;
		}
		
		if(counter==0){
			if(error){
				failfunc();
			}else{
				successfunc();
			}
		}
	}

	for(var index=0; index < instruments.length; index++){
		var identifier = instruments[index][0];
		var marketplace = instruments[index][1];
		new Ajax.Request('/mux/ajax/pluslager/instrument/hanteraInstrument.html',
		{ 
			parameters: {
				action: action,
				listid: listid,
				identifier: identifier,
				marketplace: marketplace
			},
			onComplete: function(transport) { returnhandler(transport) }
		});
	}
}

function pluslager_bevaka (formid, listid, divid, successmsg, failuremsg){
	var formelements = $(formid).serialize().split('&');
	var instruments = [];
	for(var i=0; i<formelements.length; i++){
		var formelement = formelements[i].replace(/=on/g, "");
		instruments[i] = formelement.split("_");
	}
	var error = myListInstruments('add', listid, instruments, function() { $(divid).style.display = 'none'; $(successmsg).style.display = ''; return false; }, function() { $(divid).style.display = 'none'; $(failuremsg).style.display = ''; return false;});
}

function createTrigger(identifier, marketplace, formid, onsuccess){
	
	var returnhandler = function(transport) {
		if(transport){
			var value = transport.responseText.strip();
			if(value!=1){
				alert(value);
				return false;
			}else{
				onsuccess();
				return true;
			}
		}else{
			return false;
		}
	};
	var params = $(formid).serialize() + "&identifier=" + identifier + "&marketplace=" + marketplace;
	
	new Ajax.Request('/mux/ajax/pluslager/instrument/hanteraLarm.html', { 
		parameters: params,
		onComplete: returnhandler
	});
}


function createReport(identifier, marketplace, formid, onsuccess){
	var returnhandler = function(transport) {
		if(transport){
			var value = transport.responseText.strip();
			if(value!=1){
				alert(value);
				return false;
			}else{
				onsuccess();
				return true;
			}
		}else{
			return false;
		}
	};
	var params = $(formid).serialize() + "&identifier=" + identifier + "&marketplace=" + marketplace;

	new Ajax.Request('/mux/ajax/pluslager/instrument/hanteraRapport.html', { 
		parameters: params,
		onComplete: returnhandler
	});
}

// Hämtar data från larmformuläret och anropar skript för att lägga till kurslarm.
function skapaKurslarm(plusid, divnamn, hidebackbutton){
	
	var larmform= 'larmform' + plusid;

	var trigger = $(larmform).triggerId.value;
	var villkor = ($(larmform).villkor[0].checked) ? 'LE' : 'GE';
	var mobil = ($(larmform).mobilcheckkurslarm.checked) ? $(larmform).mobilIdkurslarm.value : '';
	var mobilval = ($(larmform).mobilcheckkurslarm.checked) ? '1': '0';
	var email = ($(larmform).emailcheckkurslarm.checked) ? $(larmform).emailIdkurslarm.value : '';
	var emailval = ($(larmform).emailcheckkurslarm.checked) ? '1': '0';
	
	var plusdiv = divnamn=="plusdiv" ? plusid : divnamn;

	var parsPlus = "plusid=" + plusid + "&trigger=" + trigger + "&villkor=" + villkor + "&email=" + email + "&emailval=" + emailval + "&mobil=" + mobil + "&mobilval=" + mobilval + "&hidebackbutton=" + hidebackbutton + "&divnamn=" + divnamn;

	var url = '/mux/ajax/marknaden/aktieinformation/plus/plusSkapaKurslarm.html';
	new Ajax.Updater(plusdiv, url, {parameters:parsPlus, evalScripts:true});


}

function skapaKursrapport(plusid, divnamn, hidebackbutton){

	// Stöd för rapport via e-mail bortkomenterat.
	var rapportform = 'rapportForm' + plusid;
	
	var mobil = ($(rapportform).mobilvalrapport.checked) ? $(rapportform).mobilIdrapport.value : '';
	var mobilval = ($(rapportform).mobilvalrapport.checked) ? '1': '0';
	//var email = ($(rapportform).emailvalrapport.checked)? $(rapportform).emailIdrapport.value : '';
	//var emailval = ($(rapportform).emailvalrapport.checked) ? '1': '0';
	var morgon = ($(rapportform).morgon.checked) ? '1': '0';
	var lunch = ($(rapportform).lunch.checked) ? '1': '0';
	var kvall = ($(rapportform).kvall.checked) ? '1': '0';

	var tid = 0;
	if($(rapportform).morgon.checked){ tid += 4; }
	if($(rapportform).lunch.checked){ tid += 2; }
	if($(rapportform).kvall.checked){ tid += 1; }
	
	var plusdiv = divnamn=="plusdiv" ? plusid : divnamn;
	var parsPlus ="plusid=" + plusid + "&tid=" + tid + "&mobil=" + mobil + "&mobilval=" + mobilval + "&morgon=" + morgon + "&lunch=" + lunch + "&kvall=" + kvall + "&hidebackbutton=" + hidebackbutton + "&divnamn=" + divnamn;
	var url =  '/mux/ajax/marknaden/aktieinformation/plus/plusSkapaKursrapport.html';
	new Ajax.Updater(plusdiv, url, {parameters:parsPlus, evalScripts:true});


}
