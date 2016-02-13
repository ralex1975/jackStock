/*
 * Only plain javascript or Jquery in here. No prototype!
 * 
 * */
function showPopup(elm, execute, event) {
	if(!jQuery('#dimmerbox').length ){createDimmerbox();}
    if (jQuery.type(elm) === 'string'){elm = jQuery('#'+elm);}
    insertOnTop(elm);
    hideLightbox(elm, execute);
    jQuery(elm).css('marginLeft', '-'+leftMarginWidth(elm)+'%');
    jQuery('#dimmerbox').fadeTo(200,0.7);
    jQuery(elm).fadeIn(200);
    if(execute){jQuery(execute);}
}

// jQuery id '#someid' or obj
function fadeOutPopup(elm,execute) {
	jQuery(elm).fadeOut(100,'swing', function(){
		jQuery('#dimmerbox').fadeOut(250,'swing');
    });
    if(execute){jQuery(execute);}
}

function closePopup(elm,execute) {
    if (jQuery.type(elm) === 'string'){elm = jQuery('#'+elm);}
    jQuery('#dimmerbox').fadeOut('fast');
    jQuery(elm).fadeOut('fast');
    if(execute){jQuery(execute);}
}
//Attach event for closing popup & dimmerbox when click on dimmerbox(greyarea).
function hideLightbox(elm,execute) {
	jQuery('#dimmerbox').click(function(){
		fadeOutPopup(elm);
        if(execute){jQuery(execute);}
     });
}

//function to display element on top of dimmerbox (eg z-index for IE)
function insertOnTop(elm){
    var y = jQuery(elm).remove();
    jQuery('body').prepend(y);
}
//if jacked in via WT (has no dimmerbox)
function showDimmerbox(){
	if(!jQuery('#dimmerbox').length ){createDimmerbox();}
    jQuery('#dimmerbox').show();
}
function createDimmerbox(){
    var newdiv = document.createElement('div');
    jQuery(newdiv).hide();
    newdiv.setAttribute('id','dimmerbox');
    document.body.insertBefore(newdiv, document.body.firstChild);
}


function removeCookie(cookieName) {
	document.cookie = cookieName + "=;expires=Thu, 01 Jan 1970 00:00:01 GMT; path=/";
}
function scrollIn (elm){
	jQuery('html,body').animate({scrollTop: jQuery(elm).offset().top}, { duration: 500, easing: 'easeOutQuart' });
}
function leftMarginWidth(elm){
	return Math.round(  ( 100 * (  parseFloat(jQuery(elm).css('width')) / parseFloat( jQuery('body').css('width'))   ) / 2) );
}


// Switch between popup-boxes
function switchPopup(hidebox,showbox,execute){
	jQuery('#dimmerbox').attr('onclick','').unbind('click');
	hideLightbox(showbox,execute);
	insertOnTop(showbox);
	jQuery(showbox).css('marginLeft', '-'+leftMarginWidth(showbox)+'%');
	jQuery(hidebox).hide();
	jQuery(showbox).show();
}

function getCoord(e,rel){
	var target = e.srcElement || e.target ;
	if(rel){ // window
    	 return jQuery(target).offset();
    }else{// relative to surronding element
    	return jQuery(target).position();
    }
	return false;
}

// clones the element and returns the height
function getElementHeight(element,h){
	var clone = jQuery(element).clone();
	clone.removeAttr('id');//.css('margin-left','4000px');
	jQuery('#container').prepend(clone);//keep font-size m.m.
	var cloneHeight;
	if(h == 'max'){
		cloneHeight = clone.addClass('expanded').css('height','auto').height();
	}else{
		cloneHeight = clone.removeClass('expanded').css('height','auto').height();
	}
	jQuery(clone).remove();
	return cloneHeight;
}


//(function($) {
function slideHeight(content) {
	content = jQuery(content);
    content.toggleClass('open');
    if (content.hasClass('open')) {
    	var fullHeight = getElementHeight(content,'max');
    	content.height(content.height()); //for first animation to work
    	content.find("a:first").html("&mdash;");
    	setTimeout(function() {
    		content.height(fullHeight).addClass('expanded');
    	}, 10);
    }
    else {
    	
    	var minHeight = getElementHeight(content);
    	content.find("a:first").html("+");
    	setTimeout(function() {
    		jQuery(content).removeClass('expanded').height(minHeight);
		}, 10);
    }
    // 10ms timeout is the secret ingredient for disabling/enabling transitions
    // chrome only needs 1ms but FF needs ~10ms or it chokes on the first animation for some reason
    
}
//})(jQuery);




