/*
 * print everything within the given classname. If no classname is given elements within classname "printable" is printed!
 * 
 * 
 * */
function printElement(classname){
	
	
	var styleClass = classname+"Print";
	if(!classname || classname == "" ){
		classname = '.printable';
	}else{
		classname = '.'+classname;
	}
	
	// Grab all the style tags and copy to the new document 
	var headTags = "<head>";
	headTags += document.getElementsByTagName('head')[0].innerHTML;
	headTags += "</head>";
	
	
	//get header image
	var header = getHeader("head_");
	
	var content = "";
	//Body - collect content to be printed
	$$(classname).each(function(elm) {
		content += elm.innerHTML;
	});
	
	//for styling
	content = "<div class="+"'"+styleClass+"'>"+content+"</div>";

	//get footer
	var footer;
	
	new Ajax.Request("/mux/ajax/printFrame/printFrame.html", {
		onSuccess: function(transport) {
		footer =  transport.responseText;
		assembleAndPrint(headTags,header,content,footer);
			
		},
		onFailure: function(){
			var footerKey = "footer_";
			footer = getFooter(footerKey);
			assembleAndPrint(headTags,header,content,footer);
		}
	});
	
	
	

	
}

//assemble content and print
function assembleAndPrint(headTags,header,content,footer){
	
	//IE=document.frames  FF=iframe.contentWindow 
	var iframe = document.frames ? document.frames['printFrame'] : document.getElementById('printFrame');//IE-FF
	//get the iframe window
	var ifWin = iframe.contentWindow || iframe;//FF-IE
	//get the iframe document
	var doc = getIFrameDocument(iframe);
	
	
	//collect Content and add to iframe
	var allcontent = headTags+header+content+footer;
	doc.body.innerHTML = allcontent; 
	

	// Print the document.
	iframe.focus();
	ifWin.print();
	
}

function getHeader(key){
	var myhash = new Object();
	myhash["head_"] = '<div style="width:100%;text-align:center;margin:0 auto;"><img src="/now/images/singleLogoBlue.png"/></div>';
	
	return myhash[key] ;
}

//only used when ordinary ajax fails
function getFooter(key){
	var myhash = new Object();
	myhash["footer_"] = "<div class='print'></div>";
	return myhash[key] ;
}

//returns iframe document
function getIFrameDocument(iframe) {
    var doc;
    if (iframe.contentDocument) {//FF-Chrome
    	doc = iframe.contentDocument;
    } else if (iframe.contentWindow) {
        doc = iframe.contentWindow.document;
    } else if (iframe.document) {//IE8
        doc = iframe.document;
    } else {
        doc = window.frames[iframe.id].document;
    }

    return doc;
} 
