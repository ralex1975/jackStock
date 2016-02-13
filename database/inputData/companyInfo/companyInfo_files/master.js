var SEPARATOR      = " ";
var DECIMAL_SIGN   = ",";
var DECIMAL_PLACES = 2;

var RE_VALID_NUMBER = /^-?\d[\d\s]*([.,]\d*)?$/;

Event.observe(window, 'load', function() {
    if ($("navWrapper")) {
	Event.observe("navWrapper", 'click', stopBubble);
    }

    if ($("mc")) {
	Event.observe("mc", 'click', stopBubble);
    }

    setAutocompleteOff();
    initTrHighlight();
    
    //If we forget console.log IE7 doesnt crash
   if(!typeof console == "object"){
	   console = {log:function() {}};
   };
   if (Prototype.Browser.WebKit){ // input fields of type search renders differently in chrome.
       jQuery('body').addClass('webkit');
   }
   

});

// Stäng portalen i Firefox när man kommer till en sida via backknappen
Event.observe(window, 'pageshow', function(evnt) {
	if (evnt.persisted) {
		if (closeOpenOverlay) {
			closeOpenOverlay();
			closeOpenOverlay = null;
		}
	}
});

// Hjälpord (changed cause rightclick event triggers on 'document' in firefox)
Event.observe(window, 'load', function() {
    Event.observe(document.body, 'click', function(event) {
	if (typeof hjalpOrd != "undefined" && hjalpOrd) hjalpOrd.close();

	if (closeOpenOverlay) {
	    closeOpenOverlay();
	    closeOpenOverlay = null;
	}
    });
});

Event.observe(window, 'load', function() {
    if ($("quickbar")) quickbarWidget.init();
    if (Prototype.Browser.WebKit){ // input fields of type search renders differently in chrome.
		jQuery('body').addClass('webkit');
	}
});

// Flyttad till head /jocke
// setTimeout(function() { checkSession(false); }, 30000);

Ajax.Responders.register({
	onCreate: function(transport) {
		if (transport.container) {
			new LoadingIndicator(transport.container.success);
		}
	},
	onComplete: function(transport) {
		initTrHighlight(transport.container.success);
	}
});

function stopBubble(evnt) {
	if (!evnt) {
		evnt = window.event;
	}

	if (evnt.stopPropagation) {
		evnt.stopPropagation();
	} else {
		evnt.cancelBubble = true;
	}
}

// Per
function isIE() {
	return Prototype.Browser.IE;
}

function logBackInDiv (partnerLoginId) {

    // Load js files required by utloggad.html
    var jsFileArray = new Array('/now/js/encrypt.js?v2');
    loadJsFiles(jsFileArray);

    if (partnerLoginId != '') {
	pars = {'partnerLoginId' : partnerLoginId}
    }

    // Load utloggad.html and inject into page
    new Ajax.Updater("navWrapper", "/mux/login/utloggad.html", {
	insertion: Insertion.Before,
	evalScripts: true,
	parameters: pars
    });
}

// Per, mod av Jocke
function checkSession(hasHadSession, refresh) {
	new Ajax.Request('/mux/ajax/session/inloggad.html/auto', {
		onSuccess: function(transport) {
			if (/^\s*0\s*$/.test(transport.responseText)) {
				if (hasHadSession) {
				    if (refresh) {
					document.location.reload(true);
				    } else {
					logBackInDiv('');
				    }
				} else {
				}
			} else {
			    var response = transport.responseText;
			    if (/http|mux/.test(response)) {
				// If the response is an url, go to it.
				window.location = response;
			    } else if(/logBackIn/.test(response)){
				var getPartnerLoginId = new RegExp("([0-9]+)");
				var partnerLoginId    = getPartnerLoginId.exec(response)[0];
				logBackInDiv(partnerLoginId);
			    } else {
				ttl = parseInt(response);
				if (ttl < 5)   ttl = 5;
				if (ttl > 300) ttl = 300;
				setTimeout(function() { checkSession(true); }, ttl * 1000);
				// if you want to check every second:
				// setTimeout(function() { checkSession(true); }, 1000);
			    }
			}
		}
	});
}

// Loads several js-files synchronously starting with the first file name in jsFileArray.
// Input: String array jsFileArray
//
// Inspired by the example http://unixpapa.com/js/dyna.html
function loadJsFiles(jsFileArray) {
	if (jsFileArray.length > 0) {
		var jsFile = jsFileArray[0];

		var script = document.createElement('script');
		script.type= 'text/javascript';

		if (jsFileArray.length > 1) { // Set up a recursive call back to load the rest of the files
			jsFileArray.splice(0, 1);
			script.onreadystatechange = function () { loadJsFiles(jsFileArray); }
			script.onload = function () { loadJsFiles(jsFileArray); }
		}
		script.src = jsFile;
		$$('head').first().appendChild(script);
	}
}

// Kill me
function hideSubMenu() {}

// Per
function LoadingIndicator(layerId) {
	var width  = $(layerId).scrollWidth;
	var height = $(layerId).scrollHeight;

	if (width < 50 || height < 50) return;

	this._div = document.createElement("div");
        this._div.id               = layerId + '_spinner';
	this._div.style.marginTop  = ((height - 30) / 3) + "px";
	this._div.style.marginLeft = ((width  - 30) / 2) + "px";
        this._div.style.background = "#FFF url(/now/images/spinner.gif) no-repeat scroll center";
        this._div.style.border = "2px solid #CCCCCC";
        this._div.style.height = "30px";
        this._div.style.width = "30px";
        this._div.style.position = "absolute";
        this._div.style.zIndex = "100";

    if (layerId=='depagenerell'){
        this._div.style.left = "0%";
    }

	layerObj = $(layerId);
	layerObj.insertBefore(this._div, layerObj.firstChild);
}

LoadingIndicator.prototype.stop = function() {
	if (this._div) {
		this._div.parentNode.removeChild(this._div);
	}
};

// Per
// 2007-06-21: Kalle lade till parametern linkTexts som är en array av två strängar som ska togglas mellan (t.ex. ['visa fler', 'visa färre']).
function toggleShow(linkObj, offClass, onClass, layerId, linkTexts) {
	var layers = [ layerId ].flatten();
	for (var i = 0; i < layers.length; i++) {
	    // Check that layers[i] is either a non-null object (because typeof(null) is sadly 'object') or an existing ElementId.
	    if( (typeof(layers[i]) == 'object' && layers[i] != null) || !(!(document.getElementById(layers[i]))) ){
		if ($(layers[i]).visible()) {
			_hideShow(linkObj, offClass, layers[i]);
		} else {
			_showShow(linkObj, onClass, layers[i]);
		}
		}
	}

	if (linkTexts) {
		if (linkObj.innerHTML == linkTexts[0]) {
			$(linkObj).update(linkTexts[1]);
		} else if (linkObj.innerHTML == linkTexts[1]) {
			$(linkObj).update(linkTexts[0]);
		}
	}
}

// Per
function _hideShow(linkObj, offClass, layerId) {
	if (offClass) linkObj.className = offClass;
	$(layerId).hide();
}

// Per
function _showShow(linkObj, onClass, layerId) {
	if (onClass) linkObj.className = onClass;
	$(layerId).show();
}

// Per
function toggleShowCheckbox(checkboxObj, layerId) {
	if (checkboxObj.checked) {
		$(layerId).show();
	} else {
		$(layerId).hide();
	}
}


var closeOpenOverlay = null;
var dataSourceObj = null;


// Per
function toggleOverlay(linkObj, offClass, onClass, layerId, evnt, hideSelects) {
	if (evnt) stopBubble(evnt);

	if (linkObj.className == offClass) {
		if (closeOpenOverlay) closeOpenOverlay();
		if (hideSelects) hideSelectsIE();
		dataSourceObj = linkObj;
		_showShow(linkObj, onClass, layerId);

		Event.observe(layerId, 'click', stopBubble);

		closeOpenOverlay = function() {
			toggleOverlay(linkObj, offClass, onClass, layerId, null, hideSelects);
		};
	} else {
		closeOpenOverlay = null;
		dataSourceObj = null;
		_hideShow(linkObj, offClass, layerId);
		if (hideSelects) showSelectsIE();
	}
}


// Kraftigt modifierad av Per, amenderad med directSearch av Anders 2008-01-18
function sokAktier(url, extraParams, directSearch) {
	var pars
	if (directSearch == null) {
		if ($F('search') == "") return;

		//search for ETF´s must be converted
		var vardepapper = $F('vardepapper');
		if(vardepapper == 'ETF' || vardepapper == 'ETN' || vardepapper == 'ETC'){
			vardepapper = 'A';
		}
		// utan det här krashar sökningen om någon matar in ?
		pars = "marknad=" + encodeURIComponent( $F('marknad') ) + "&kind=" + encodeURIComponent( vardepapper ) + "&search=" + encodeURIComponent( $F('search') );
		if (extraParams != null) {
			pars = pars + "&" + extraParams;
		}
	} else {
		pars = extraParams;
	}

	if ($('localLoadingIndicator')) $('localLoadingIndicator').show();

	if (closeOpenOverlay) closeOpenOverlay();

	new Ajax.Updater('seekResults', url, {
		parameters: pars,
		evalScripts: true,
		onSuccess: function() {
			hideSelectsIE("orderlaggning");
			$('seekResults').show();

			Event.observe('seekResults', 'click', stopBubble);

			closeOpenOverlay = function() { $('seekResults').hide(); showSelectsIE(); };

			if ($('localLoadingIndicator')) {
				$('localLoadingIndicator').hide();
			}
		}
	});
}


// Per
function switchTab(linkObj, layerId, ajaxUrl, ajaxOptions) {
	// Plocka bort class från alla flikar (<li>)
	var lis = linkObj.parentNode.parentNode.getElementsByTagName("li");

	for (var i = 0; i < lis.length; i++) {
		$(lis[i]).removeClassName("current");
	}

	// Markera vald flik
	$(linkObj.parentNode).addClassName("current");

	// Om lagret som ska visas är dolt så antar vi att något annat syns
	// istället och ska döljas först
	if ($(layerId).style.display == "none") {
		_hideSiblings(layerId);

		$(layerId).show();
	}

	// Hämta innehåll
	if (ajaxUrl) {
		new Ajax.Updater(layerId, ajaxUrl, { parameters: (ajaxOptions || {}), evalScripts: true });
	}
}

// Formaterar nummer med rätt decimaltecken och tusenavdelare

function formatNumber(number, decimalPlaces, showZeroFractions) {
	if (isNaN(number)) return undefined;

	var fourDigits = new RegExp("^([^.,]*[0-9])([0-9]{3})([" + SEPARATOR + ".,]|$)");

    if (decimalPlaces === 0 || decimalPlaces > 0) {
    	result = number.toFixed(decimalPlaces);
    } else {
        result = number.toString();
    }

    if (!showZeroFractions) result = result.replace(/\.0+$/, "");

	while (fourDigits.test(result)) {
	    result = result.replace(fourDigits, "$1" + SEPARATOR + "$2$3");
	}

	result = result.replace(/\./, DECIMAL_SIGN);

	return result;
}

// Per
function SokWidget(marketId, typeId, instruments, markets, showSingleOption, wrapperId) {
	this.instruments = instruments;

	this.marketSelectId = marketId;
	this.typeSelectId   = typeId;
	if (typeof wrapperId != 'undefined') {
		this.wrapperId = wrapperId;
	}
	else {
		this.wrapperId = null;
	}

	this.markets = {
		Sverige:  [ "A", "FND", "O", "WNT", "Onoterade" ],
		Norge:    [ "A", "FND", "O", "WNT", "Onoterade" ],
		Danmark:  [ "A", "O" ],
		Finland:  [ "A", "WNT" ],
		Tyskland: [ "A", "WNT" ],
		USA:      [ "A" ],
		Kanada:   [ "A" ]
	};

    if (markets)this.markets = markets;


    this.showSingleOption = showSingleOption;

}

//Added for ETF-Center optional preselectC & preselectT
SokWidget.prototype.rebuild = function(preselectC,preselectT) {

	if ($F(this.marketSelectId) == "undefined" || $F(this.marketSelectId).length == 0) return;

	//get validInstruments(from .view or this obj)
	var marketInstruments = this.markets[$F(this.marketSelectId)];
	var validInstruments  = $A(marketInstruments).findAll(_isValid.bind(this));
	var options  = $(this.typeSelectId).options;
	var selected = $F(this.typeSelectId);


	//set type ETF
	if(preselectT != undefined){
		selected = preselectT;
	}
	//set market ETF
	if(preselectC != undefined){
		var someNodeList = $(this.marketSelectId).getElementsByTagName('option');
		var nodes = $A(someNodeList);
		nodes.each(function(node){
				if(node.value == preselectC){
					node.selected = true;
				}
			});
	}


	if(selected == 'FND')removeMarket();

	options.length = 0;


    if (validInstruments.length > 1) {
    	validInstruments.each(_addOption.bind(this));
        if (this.wrapperId != undefined){
			$(this.wrapperId).show();
        }
        else {
			$(this.typeSelectId).show();
        }
    } else {
    	_addOption(validInstruments[0]);
		if (!this.showSingleOption) {
	        if (this.wrapperId != undefined){
				$(this.wrapperId).hide();
	        }
	        else {
				$(this.typeSelectId).hide();
	        }
		}
    }


    function _isValid(inst) {
		return !!this.instruments[inst];
	}

	function _addOption(inst) {
		options[options.length] = new Option(this.instruments[inst], inst, false, (inst == selected));
	}
};


// Per
function HandelsdagarWidget(marketId, dateId, dates) {
	this.dates = dates;

	this.marketSelectId = marketId;
	this.dateSelectId   = dateId;
}

HandelsdagarWidget.prototype.rebuild = function() {
	var marketDates = this.dates[$F(this.marketSelectId)];

	var options  = $(this.dateSelectId).options;
	var selected = $F(this.dateSelectId);

	options.length = 0;

    marketDates.each(_addOption.bind(this));

	function _addOption(datum) {
		options[options.length] = new Option(datum, datum, false, (datum == selected));
	}
};


// Växlar innehåll i hjälpfliken till vänster på exempelvis handlasidorna
// Per
function showHelp(helpDivId) {
	// Gör om exempelvis 'antal' till 'helpAntal'
	helpDivId = "help" + helpDivId.capitalize();

	// Göm alla andra hjälpavsnitt först
	_hideSiblings(helpDivId);

	$(helpDivId).show();
}


/* Meddelandecentralen (some functions, not all!)*/



// Per foo 486
function toggleMc(url, params, linkId) {

	new Ajax.Updater(
		'mcContainer', url,
		{
			parameters: params,
			evalScripts: true,
			onSuccess: function() {
				hideSelectsIE();
				if (closeOpenOverlay) closeOpenOverlay();
				showMc();

				if ($(linkId)) $(linkId).addClassName('current');
				closeOpenOverlay = function() {
					if ($(linkId)) $(linkId).removeClassName('current');
					saveMsgContent();
					showSelectsIE();
					closeOpenOverlay = null;
				};
			}
		}
	);
}

function toggleAjaxContainer(container_id, url, params, linkId) {

	new Ajax.Updater(container_id, url, {
			parameters: params,
			evalScripts: true,
			onSuccess: function() {
				hideSelectsIE();
				if (closeOpenOverlay) closeOpenOverlay();
				showMc();

				if ($(linkId)) $(linkId).addClassName('current');
				closeOpenOverlay = function() {
					if ($(linkId)) $(linkId).removeClassName('current');
					saveMsgContent();
					showSelectsIE();
					closeOpenOverlay = null;
				};
			}
		}
	);
}


function closeMc(){
	saveMsgContent();
	closePopup('mc');
	showSelectsIE();
	closeOpenOverlay = null;
}
function showMc(){
	showPopup('mc');
	jQuery('#dimmerbox').click(function(){ closeMc; } );
}

// Check or uncheck all Radiobuttons within given element wrapper (only mandatory arg is wrapperElm).
function checkAll(topElm,wrapperElm,state){
	if(!state){state = topElm.checked;}
    $$('#'+wrapperElm+' input').each(function(e) {e.checked = state});
}
//returns true if all is checked within given wrapper element
function allIsChecked (wrapperElm){
	var status = true;
	$$('#'+wrapperElm+' input').each(function(e) {
		if( e.checked == false){
			status = false;
			throw $break;
		}
	});

	return status;
}
/* meddelandecentralen END*/


// Visa/dölj alla <select> för IE när så önskas
// Per
(function() {
	var _hideCount = 0;

	hideSelectsIE = function(containerId) {
		if (!isIE()) return;

		if (_hideCount == 0) _setSelectVisibility("hidden", containerId);
		_hideCount++;
	};

	showSelectsIE = function() {
		if (!isIE()) return;
		_hideCount--;
		if (_hideCount <= 0) {
			_setSelectVisibility("visible");
			_hideCount = 0;
		}
	};

	function _setSelectVisibility(visibility, containerId) {
	    var selects;

        if (containerId) {
    		selects = $$("#" + containerId + " select");
        } else {
    		selects = $$("select");
        }

		for (var i = 0; i < selects.length; i++) {
			selects[i].style.visibility = visibility;
		}
	}
})();

// Per
function setAutocompleteOff() {
	var inputs = document.getElementsByTagName('input');

	for (var i = 0; i < inputs.length; i++) {
		if (inputs[i].type == "text") {
			inputs[i].setAttribute("autocomplete", "off");
		}
	}
}

// Per
function initTrHighlight(scopeId) {
	var scope = $(scopeId) || $('content');

    if (scope === null) return;

	var tables = scope.getElementsByTagName('table');

	for (var i = 0; i < tables.length; i++) {
		var table = tables[i];
        if (table.getElementsByTagName('th').length > 1 &&
            table.getElementsByTagName('tr').length > 4) {
			    _setTrHighlight(table);
        }
	}

	function _setTrHighlight(tableObj) {
		var trs = tableObj.getElementsByTagName('tr');

		for (var i = 0; i < trs.length; i++) {
			trs[i].onmouseover = function() { this.style.backgroundColor = "#DDD"; };
			trs[i].onmouseout  = function() { this.style.backgroundColor = ""; };
		}
	}
}


// Per
function checkForReturn(formObj, evnt) {
	var keyCode = window.event ? window.event.keyCode : evnt.which;

	if (keyCode == 13) {
		if (formObj.onsubmit) {
			formObj.onsubmit();
		} else {
			formObj.submit();
		}
	}
}

// Per
function checkAgreement(containerId, offClass, onClass, buttonId) {
	var allGood = true;
	var agreements = $(containerId).getElementsByTagName('input');

	for (var pos = 0; pos < agreements.length; pos++) {
		if (!agreements[pos].checked) {
			allGood = false;
			break;
		}
	}

	var buttonLink = $(buttonId).getElementsByTagName('a')[0];

	if (allGood) {
		$(buttonId).className = onClass;
		buttonLink.onclick = function() { return true; };
	} else {
		$(buttonId).className = offClass;
		buttonLink.onclick = function() { return false; };
	}
}


// Per
function sortera(kolumn, sorteringsordning) {
	// Plocka ut adressraden efter frågetecknet
	var params = document.location.search;

	// Gör om params till ett objekt
	var paramsObj = params.toQueryParams();

	// Ändra eller skapa parameter för sorteringsordning
	if (sorteringsordning) {
		paramsObj.sorteringsordning = sorteringsordning;
	} else {
		paramsObj.sorteringsordning = "fallande";
	}

	// Ändra eller skapa parameter för sorteringskolumn
	paramsObj.sortera = kolumn;

	// Omvandla tillbaka objektet och ladda om sidan med nya params
	window.location.search = ($H(paramsObj).toQueryString());
}

function showPreview(outputId, content, event, parent_container) {
	// Optional parameter for the reference parent container
	if(!parent_container){
		parent_container = 'container';
	}

    var mouseX = (event.pageX || event.clientX + document.documentElement.scrollLeft);
    var mouseY = (event.pageY || event.clientY + document.documentElement.scrollTop);

    var con = $(parent_container);
    var output = $(outputId);

    var offsetX = con.offsetLeft;
    var offsetY = con.offsetTop;

    output.innerHTML = content;
    output.style.top = (mouseY - offsetY + 20) + 'px';
    output.style.left = (mouseX - offsetX - output.getWidth() / 2) + 'px';

    output.show();
}

// Per
function iframeResize() {
	var iframe = window.frames[0].document.documentElement;
	$('morningstar').style.width  = iframe.scrollWidth  + "px";
	$('morningstar').style.height = iframe.scrollHeight + "px";
}

// Per
function _hideSiblings(layerId) {
	var sibling = $(layerId).parentNode.firstChild;

	for (; sibling; sibling = sibling.nextSibling) {
		if (sibling.nodeName == "DIV" && !Element.hasClassName(sibling, 'doNotHide') ) {
			$(sibling).hide();
		}
	}
}



//Diverse funktioner. (Christer och Tobias (C)2006)
var globalNewsIngressSwitchToTheLeft=false;

//Byta till realtidsikon.
//FIXME
function changeIkon(namn){
var ikon = document.getElementById(namn);
ikon.src='images/ikon_realtid.gif';
}

/////////////////////////////////////

var allaLagers = new Array();

//FIXME
function closeLayers(namn,namn2){
var divs=document.getElementsByTagName("DIV");
//var links = document.getElementsByTagName('a');
	for(var i=0; i < divs.length; i++){
		if(divs[i].className.indexOf("closeLayer") != -1 && divs[i].getAttribute('id')!=namn){
		divs[i].style.display = "none";
		}
	}
	for(var y=0; y < allaLagers.length; y++){
		if (allaLagers[y]!=namn2){
		document.getElementById(allaLagers[y]).src='images/dropdown_pil2_ner.gif';
		}
	}
}
////////////////////////////////////////


// För att byta flikar
//FIXME
function bytTillFlik(id)
{
	var e = document.getElementById(id);	// <li>
	var s = e.parentNode.firstChild;
	while (s != null) {
		if (s.className != null) {
			s.className = s.className.replace(/\bcurrent\b/, "");
		}
		s = s.nextSibling;
	}
	e.className="current";
}


// Att visa/gomma Lager, ange bara lagernamn.
//FIXME
function showL(namn){
	var bild = document.getElementById(namn).style;
	closeLayers();
	if(bild.display!=null){
		bild.display='block';
	}
	if(bild.visibility!=null){
		bild.visibility='visible';
	}
}

//Att visa/gomma Lager där lagret placeras i höjd/bredd-ledd i förhållande till vart länken är. Används för tillfället på makrnadsöversikten för att visa
//nyheter. (Tobias)
//FIXME
function showLWithTopSense(namn,event){
	var layerStyle = document.getElementById(namn).style;
	closeLayers();
	var mouseX=0;
	var newX=1;
	if(isIE()){
		mouseX=event.x+2;
		if(mouseX>200){
			newX=0;
		}
	}
	else{
		mouseX=event.pageX;
		if(mouseX>600){
			newX=0;
		}
	}
	var newTop=55;
	layerStyle.top=newTop+"px";
	if(newX==0){
		layerStyle.left=newX+"px";
	}
	if(layerStyle.display!=null){
		layerStyle.display='block';
	}
	if(layerStyle.visibility!=null){
		layerStyle.visibility='visible';
	}
}

//FIXME
function hideL(namn){
	if (document.getElementById(namn)) {
		var bild = document.getElementById(namn).style;
		bild.display='none';
	}
}


function blurLinks() {
}


//Används för att visa/släck lager eller tabell-rader. Valfri text bredvid knappen. ifReversible ska användas om lagret/raderna inte ska kunna "tas bort" igen; alltså
// ingen plus-minus knapp.
//(Tobias V)
//FIXME
function extendedInfoGeneral(nameToHide,plusLinkId,plusLinkText,minusLinkText,plusClass,minusClass,ifReversible){
	var moreLink=null;
	var links = $$("a");
	//Ändra länktexten bredvid plus/minus-knappen
	if(plusLinkText!=minusLinkText){
		if(document.getElementById(plusLinkId).innerHTML!=plusLinkText){
			document.getElementById(plusLinkId).innerHTML=plusLinkText;
			document.getElementById(plusLinkId).className=plusClass;
			moreLink=$(plusLinkId);
		}
		else{
			document.getElementById(plusLinkId).innerHTML=minusLinkText;
			document.getElementById(plusLinkId).className=minusClass;
			moreLink=$(plusLinkId);
		}
	}
	else{
		for(var i=0;i<links.length;i++){
			if(links[i].id==plusLinkId){
				if(!links[i].hasClassName('minus')){
					links[i].setAttribute("class", minusClass);
					links[i].setAttribute("className", minusClass);
					moreLink=links[i];
				}
				else{
					links[i].setAttribute("class", plusClass);
					links[i].setAttribute("className", plusClass);
					moreLink=links[i];
				}
				break;
			}
		}
	}
	var allDivs = $$("tr");
	var ifDiv=true;
	var ifTR=false;
	for(var i=0;i<allDivs.length;i++){
		div=allDivs[i];
		if(div.getAttribute("name")==nameToHide){
			if(!moreLink.hasClassName("minus")){
				div.hide();
				div.className="";
			}
			else if(!moreLink.hasClassName('plus')){
				div.show();
				div.className="";
			}
			ifDiv=false;
		}
	}
	//Icke reversibelt
	if(!ifReversible){
		$(plusLinkId).style.display='none';
	}
	//Om lagret som ska gömmas är ett div-fält
	if(ifDiv&&ifReversible){
		div=$(nameToHide);
		if(!moreLink.hasClassName('minus')){
			div.hide();
		}
		else if(!moreLink.hasClassName('plus')){
			div.show();
		}
	}
}

//FIXME
function extendedInfo(id,id2){
	var lager = document.getElementById(id2).style;
	id.className=(id.className!="minus")? "minus" : "plus";
	lager.display=(lager.display=="none")? "" : "none";
}

//Plus-minus-knapp utan text. (Tobias V)
//FIXME
function extendedWithoutTextChange(divToExtend,plusButtonId,plusLinkId){
	var bild = document.getElementById(plusButtonId);
	var div = document.getElementById(divToExtend);
	if(bild.src.indexOf("minus")!=-1){
		div.style.display = "none";
	}
	else if(bild.src.indexOf("plus")!=-1){
		div.style.display = "";
	}
	if(bild.src.indexOf("minus")!=-1){
		bild.src='images/knapp_plus_small.gif';
	}
	else if(bild.src.indexOf("plus")!=-1){
		bild.src='images/knapp_minus_small.gif';
	}
}

//Hide table rows. (Tobias V)
//FIXME
function hideTableRows(nameToHide){
	var allDivs = document.getElementsByTagName("tr");
	for(var i=0;i<allDivs.length;i++){
		div=allDivs[i];
		if(div.getAttribute("name")==nameToHide){
			div.style.display = "none";
		}
	}
}

// Script för topnavigation Reklam och Hjälp.
function showNavLayer(helpLayer,helpIcon) {
var lager = document.getElementById(helpLayer);
var bild = document.getElementById(helpIcon);
lager.style.display=(lager.style.display!="none")? "none" : "block";

	if (bild.src.indexOf("minus")!= -1){
	bild.src='/now/images/knapp_plus_small.gif';
	}
	else if (bild.src.indexOf("plus")!= -1){
	bild.src='/now/images/knapp_minus_small.gif';
	}

	if (bild.src.indexOf("Green")!= -1){
	bild.src='/now/images/ikonHelpWhite.gif';
	}
	else if (bild.src.indexOf("White")!= -1){
	bild.src='/now/images/ikonMinusGreen.gif';
	}
}


// Classic

function wtproblem(service, lang) {
    ModalWin12 =
    window.open("/tux/lang/f/problemwt.pl?service=" + service + "&lang=" + lang, "ModalWin12","toolbar=no,location=0,directories=no,scrollbars=yes,resizable=yes,width=418,height=660");
}

function popup(mypage,myname,w,h,scroll,pos){
    if(pos=="random"){LeftPosition=(screen.width)?Math.floor(Math.random()*(screen.width-w)):100;TopPosition=(screen.height)?Math.floor(Math.random()*((screen.height-h)-75)):100;}
    if(pos=="center"){LeftPosition=(screen.width)?(screen.width-w)/2:100;TopPosition=(screen.height)?(screen.height-h)/2:100;}
    else if((pos!="center" && pos!="random") || pos==null){LeftPosition=0;TopPosition=20;}
    settings='width='+w+',height='+h+',top='+TopPosition+',left='+LeftPosition+',scrollbars='+scroll+',location=no,directories=no,status=no,menubar=no,toolbar=no,resizable=no';

    win=window.open(mypage,myname,settings);
    if(win.focus){win.focus();}
}

// För att kunna resizea QuickTradefönstret i NOW (resizeable=yes). Jag skapar en egen funktion för att förebygga legacyproblem. /Anders 2007-08-06
function popupQuickTrade(mypage,myname,w,h,scroll,pos){
    if(pos=="center"){LeftPosition=(screen.width)?(screen.width-w)/2:100;TopPosition=(screen.height)?(screen.height-h)/2:100;}
    settings='width='+w+',height='+h+',top='+TopPosition+',left='+LeftPosition+',scrollbars='+scroll+',location=no,directories=no,status=no,menubar=no,toolbar=no,resizable=yes';

    win=window.open(mypage,myname,settings);
    if(win.focus){win.focus();}
}

function bestillinfo() {
    ModalWin2 = window.open("/tux/lang/bestallinfo/via_post.pl","ModalWin2","toolbar=yes,location=0,directories=no,scrollbars=yes,resizable=yes,width=586,height=560");
}

function blanketter() {
    ModalWin29 = window.open("/NNNO/no/k/blanketter.html","ModalWin29","toolbar=no,location=0,directories=no,scrollbars=yes,resizable=yes,width=417,height=705");
}

function blikunde() {
    ModalWin1 = window.open("/mux/web/nordnet/blikund.html","ModalWin1","toolbar=yes,location=0,directories=no,scrollbars=yes,resizable=yes,width=670,height=650");
}

function prisliste() {
    ModalWin4 = window.open("https://www.nordnet.no/NNNO/no/k/prisliste.html","ModalWin4","toolbar=no,location=0,directories=no,scrollbars=yes,resizable=yes,width=760,height=500");
}

// Till bannern på svenska förstasidan
function fixedSize(url,name,features) {
		win = window.open(url,name,features);
}

function maybeQuicktrade(handelsmetod, identifier, marketid, kopsalj, search, vol){
	if(handelsmetod=="quicktrade"){
		var params = "";
		if(kopsalj.length + identifier.length + marketid.length>0){
			if ( search )
			{
				params = "?kopsalj=" + kopsalj + "&search=" + identifier + "&marknad=" + marketid + "&vol=" + vol;
			}
			else
			{
				params = "?kopsalj=" + kopsalj + "&identifier=" + identifier + "&marketid=" + marketid + "&vol=" + vol;
			}
		}
		popupQuickTrade("/mux/page/quicktrade.html" + params, 'QuickTrade5','518','550','yes','center');
		return false;
	}else{
		return true;
	}
}

// Jocke
// Closure alert
function LoginWidget(formId, useEncryption, key, sessionid)
{
	this.refreshing = false;
	this.queueSubmit = false;
	this.formRefresher = null;
	this.loginFormId = formId;
	this.useEncryption = useEncryption;
	this.key = key;
	this.sessionid = sessionid;

	this.start = function()
	{
		this.formRefresher = new PeriodicalExecuter( this.formRefresh.bind(this), 600 );
	}

	this.loggaIn = function()
	{
		var loginForm = $(this.loginFormId);
		if ( this.formRefresher )
			this.formRefresher.stop();

		if ( !this.refreshing ) {
			if (this.useEncryption != 0) {
				var keyObj = RSA.getPublicKey(this.key);
				$('pContHidden').value = RSA.encrypt($F('pContent'), keyObj, this.sessionid);
			}
			loginForm.submit();
		}
		else {
			this.queueSubmit = true;
		}

		return false;
	};

	this.formRefresh = function( pe )
	{
		this.refreshing = true;

		new Ajax.Request( '/mux/login/refresh.js', {
			onSuccess: function(transport)
			{
				var loginForm = $(this.loginFormId);
				var hash = eval( '$H(' + transport.responseText + ')' );
				hash.each(function(pair) {
					var element = loginForm.getInputs( null, pair.key ).first();
					if ( element )
						element.name = pair.value;
				});
				if (this.queueSubmit) loginForm.submit();
				this.refreshing = false;
			}.bind(this)
		});
	};
}


// Kristoffer
function addGraphInstrument(loc, identifier, marketplace){
	loc += loc.indexOf("?")==-1 ? "?" : "&";
	loc += "graphid=" + identifier + "_" + marketplace;
	loc.replace(/#/g,"");
	window.location.href = loc;
}

function removeGraphInstrument(loc, identifier, marketplace){

	var parobj = $H(loc.toQueryParams());
	var graphid = parobj.get('graphid');
	var newurl = loc.substring(0,loc.indexOf("?"));
	if(graphid){
		if (graphid[0].length > 1) {
			graphid = $A(parobj.get('graphid')).flatten();
			graphid = graphid.without(identifier + "_" + marketplace);
			parobj.set('graphid', graphid);
		} else {
			parobj.unset('graphid');
		}

		newurl += "?" + parobj.toQueryString();

	}
	window.location.href = newurl;
}



// Anders adds a nifty function he found
function getOldPageWithRefresh(){
	var newPage = document.referrer;
	if(newPage.length==0){
		history.go(-1);
	}else{
		newPage += newPage.indexOf("?")==-1 ? "?" : "&";
		if(newPage.indexOf("forceupdate") > 0) {
			newPage = newPage.substring(0, newPage.indexOf("forceupdate="));
		}
		newPage += "forceupdate="+Math.random();
		window.location = newPage;
	}
	return false;
}

//macro measure for sitecatalyst
function setSiteCatalystCmpi (name,country) {
	try{
		if(typeof(s) == 'object'){
	 		s.cmpi = name;
	 		if(country){s.cmpi+="-"+country}
	 	}
	}catch(e){}
}
//Save email e.t.c for different product inquiry
function productInquiry(error,type){

	if(validateEmail($F('email') )){
		var params = $('productInquiry').serialize();
		params += '&type='+type;
		new Ajax.Updater("PIContainer", "/mux/ajax/common/productInterestReg.html", {
			parameters: params
		    });

	}else{
		$$('.warning').invoke('update', error);
	}
}

function validateEmail(email) {
	var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
	return reg.test(email);
}

document.tradeAgreementsLoaded = {};
document.tradeAgreementsCreated = {};

// launch a popup with content fetched from given id
function showAgreement (id,params){
	var marketplace = params['marketplace'];
	var elementId = id + '_' + marketplace;

	if (document.tradeAgreementsCreated[marketplace]) {
		if (document.tradeAgreementsLoaded[marketplace]) {
			showPopup(elementId);
		}
	} else {
		document.tradeAgreementsCreated[marketplace] = true;

		loadCSS('/now/css/dynamic/agreement.css');

		var params = $H( params ).toQueryString();
		params += "&id="+id;

		new Ajax.Request('/mux/ajax/common/popup/'+id+'.html', {
			parameters: params,
			evalScripts:true,
			onSuccess: function(transport) {
		    	var result = transport.responseText;
		    	var elm = document.createElement('div');
		    	$(elm).update(result);
		    	document.body.insertBefore(elm, document.body.firstChild); //will leave an empty div :(

		    	showPopup(elementId);
		    	$(elementId).scrollIntoView(true);

		    	document.tradeAgreementsLoaded[marketplace] = true;
		    }
	    });
	}
}

//load css
function loadCSS(cssName){

	if(!document.cssLoaded){

		//add css to head
		var headID = document.getElementsByTagName("head")[0];
		var cssNode = document.createElement('link');
		cssNode.type = 'text/css';
		cssNode.rel = 'stylesheet';
		cssNode.href = '/now/css/dynamic/agreement.css';
		cssNode.media = 'screen';
		headID.appendChild(cssNode);
		document.cssLoaded = true;
	}
}

// Check if sessionStorage is supportded
function sessionStorageSupport() {
	try {
		return 'sessionStorage' in window && window['sessionStorage'] !== null;
	}catch (e) {
		return false;
	}
}

// Get parameters from url(window.location).
// Ex: qs["identifier"]
var qs = (function(a) {
    if (a == "") return {};
    var b = {};
    for (var i = 0; i < a.length; ++i)
    {
        var p=a[i].split('=');
        if (p.length != 2) continue;
        b[p[0]] = decodeURIComponent(p[1].replace(/\+/g, " "));
    }
    return b;
})(window.location.search.substr(1).split('&'));
