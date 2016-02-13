// If sessionStorage is supported, subject and message content is saved in local browser. 
function saveMsgContent(){
	
	//FAQ
	if(sessionStorageSupport()){
		if($('maincatid')){
			var answerKey = 'mittsvar_'+$('maincatid').value;
			var errandKey = 'errand_'+$('maincatid').value;
			if($('mittsvar') && $('mittsvar').value){
				sessionStorage.setItem(answerKey,$('mittsvar').value );
			}
			if($('errand') && $('errand').value){
				sessionStorage.setItem(errandKey,$('errand').value );
			}
			
		}else{//regular answer. Get is made directly in mcSvara.view (not getMsgContent)
			if($('mittsvar') && $('unid') && $('mittsvar').value){
				sessionStorage.setItem($('unid').value,$('mittsvar').value );
			} 
		}
		
	}
	
}
// Get locally stored msg. 
function getMsgContent(){
	
	if(sessionStorageSupport()){
		if($('maincatid')){
			var answerKey = 'mittsvar_'+$('maincatid').value;
			var errandKey = 'errand_'+$('maincatid').value;
			if($('mittsvar') && sessionStorage.getItem(answerKey) != null && sessionStorage.getItem(answerKey) != ""){
				$('mittsvar').value = sessionStorage.getItem(answerKey);
			}
			if($('errand') && sessionStorage.getItem(errandKey) != null && sessionStorage.getItem(errandKey) != ""){
				$('errand').value = sessionStorage.getItem(errandKey);
			}
		}
	}
}

function deletMsgContent(key){
	if(sessionStorageSupport()){
		sessionStorage.removeItem(key);
	}
}

// update displaydata(no of messages in inbox .e.t.c)
function updateMessageCenter(olastatext, image, mc_class){
	if($('olastatext')){ $('olastatext').update(olastatext);}
	if($('olastatext')){ $('olastatext').className=mc_class;}
	if($('mc_image')){ $('mc_image').src=image;}
}
//mcFaqShow.view
function displayForm(){
	$('kontaktMeddelande').hide(); 
	$('kontaktFormular').show();
	getMsgContent();
}
//Per
function toggleMcSize() {
	$("mc").toggleClassName("maximize");
	mcShake();
}
// Per
function mcShake() {
	$("mc").style.paddingLeft = "2px";
	$("mc").style.paddingLeft = "1px";
	$("mc").style.paddingLeft = "";
}
//Per
function mcTeckenKvar(max, errorMsg) {
	var teckenKvar = +max - $F("mittsvar").length;
	$("teckenKvar").innerHTML = teckenKvar;

	if (teckenKvar < 0) {
		$("mittsvar").value = $F("mittsvar").slice(0, max);
		alert(errorMsg);
	}
}
// Function to display element on top of dimmerbox (eg z-index for IE)
function displayOnTop(elm){
	var y = $(elm).remove();
	document.body.insertBefore(y, document.body.firstChild);
	$('dimmerbox').show();
	$(elm).show();
}
// Functions to enable and disable form-elements
function disableElement(id) {
	document.getElementById('filen').setAttribute('disabled', 'disabled');
}
function appendFileLoader(container_id) {
	$('uploadHalt').hide();
	new Insertion.After('uploadFeedback', "<img id='loadingImg'src='/now/images/loadingAnimation.gif'/>");
}



function setCatHeader(catId) {

	for(var i = 0; document.getElementById('cathead_' + i); i++) {
		var catid_element	= document.getElementById('cathead_' + i);
		catid_element.className	= '';
	}
	var catHeader		= document.getElementById(catId);
	catHeader.className	= 'mcCurrent';
}

