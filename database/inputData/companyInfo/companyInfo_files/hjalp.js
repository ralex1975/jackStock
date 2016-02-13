var AjaxKeys =
{
	set: function( key, value, persistent )
	{
		new Ajax.Request(
			'/mux/ajax/setAjaxKey.html',
			{
				parameters:
				{
					key: key,
					value: value,
					longevity: (persistent ? 'persistent' : 'temporary')
				}
			}
		);
	}
};

function snabblogin(username)
{
    if (closeOpenOverlay) { closeOpenOverlay(); }

    new Ajax.Request(
        '/mux/ajax/session/snabblogin.html',
        {
            parameters: { username: username },
            onSuccess: redirect
        }
    );
}

function bytDepa(portfolio, onSuccess)
{
    if (closeOpenOverlay) { closeOpenOverlay(); }

    new Ajax.Request(
        '/mux/ajax/session/bytdepa.html',
        {
            parameters: { portfolio: portfolio },
            onSuccess: onSuccess || redirect
        }
    );
}


function warnKyc(portfolio){
	showPopup('warn-kyc');
	jQuery('#warn-kyc-btn').click(function(){
		bytDepa(portfolio);
		switchAccountWidget.closeOverlay();
		return false;
	});
}

function redirect(transport) {
	var uri = transport.responseText.match(/^([^<>\n]+)\n*$/);

	if (uri && uri[1]) {
		if (document.location.href.indexOf(uri[1] ) != -1) {
			document.location.reload();
		} else {
			document.location.href = uri[1];
		}
	}
}

function login( params )
{
	if ($('loginErrorMessage')) $('loginErrorMessage').hide();
	if ($('localLoadingIndicator')) $('localLoadingIndicator').show();

	new Ajax.Request(
		'/mux/ajax/session/login.html',
		{
			parameters: params,
			onSuccess: function( transport )
			{
				var uri = transport.responseText.match( /^([^<>\n]+)\n*$/ );
				if ( uri && uri[1] )
				{
					document.location.href = uri[1];
				}
				else
				{
					if ($('localLoadingIndicator')) $('localLoadingIndicator').hide();
					if ($('loginErrorMessage')) $('loginErrorMessage').show();
				}
	                },
			onFailure: function( transport )
			{
				if ($('localLoadingIndicator')) $('localLoadingIndicator').hide();
				if ($('loginErrorMessage')) $('loginErrorMessage').show();
			}
		}
	);

	return false;
}

function logout()
{
	new Ajax.Request(
		'/mux/ajax/session/logout.html',
		{
			onSuccess: function( transport )
			{
				removeCookie('NEXT');
				removeCookie('WT_NOW');
				var uri = transport.responseText.match( /^([^<>\n]+)\n*$/ );
				if ( uri && uri[1] )
				{
					document.location.href = uri[1];
				}
			}
		}
	);

	return false;
}

function classicPopup(mypage,myname,w,h,scroll)
{
	LeftPosition=(screen.width)?(screen.width-w)/2:100;
	TopPosition=(screen.height)?(screen.height-h)/2:100;
	settings='width='+w+',height='+h+',top='+TopPosition+',left='+LeftPosition+',scrollbars='+scroll+',location=no,directories=no,status=no,menubar=no,toolbar=no,resizable=no';
	window.open(mypage,myname,settings);

	return false;
}

function valjMarknad( sokord, marknad, kopsalj, typ, volym )
{
	var marknadObj, vardepapperObj, searchObj, fastSeek = 0;
	if ( $('marknad') )
	{
		marknadObj     = $('marknad');
		vardepapperObj = $('vardepapper');
		searchObj      = $('search');
	}
	else
	{
		marknadObj     = $('fastSeekMarknad');
		vardepapperObj = $('fastSeekKind');
		searchObj      = $('fastSeekInput');
		fastSeek = 1;
	}

	for ( var index = 0; index < marknadObj.options.length; index++ )
		if ( marknadObj.options[index].value == marknad )
			break;

	if ( index >= marknadObj.options.length )
		return;

	if ( closeOpenOverlay ) { closeOpenOverlay(); closeOpenOverlay = null; }

	marknadObj.selectedIndex = index;

	if ( typ != null )
	{
		for ( index = 0; index < vardepapperObj.options.length; index++ )
			if ( vardepapperObj.options[index].value == typ )
				break;

		if ( index >= vardepapperObj.options.length )
			return;

		vardepapperObj.selectedIndex = index;
	}
	else
	{
		vardepapperObj.selectedIndex = 0;
	}

	searchObj.value = sokord;

	if ( !fastSeek )
	{
		if ( kopsalj == 'stoploss' )
			sokAktier( '/mux/ajax/handla/sokresultat.html', 'stoploss=stoploss' );
		else
			sokAktier( '/mux/ajax/handla/sokresultat.html', 'kopsalj=' + kopsalj + ( volym != null ? '&vol=' + volym : '' ) );
	}
	else
	{
		return supportNavWidget.search(kopsalj);
	}

	return false;
}

function openHjalpOrd( ord, evnt )
{
	stopBubble( evnt );

	if ( closeOpenOverlay ) closeOpenOverlay();

	var hjalpOrd = window.open( '/mux/page/hjalp/ordHjalp.html?ord=' + ord, 'nn_hjalpord', 'height=400,width=350,dependent=yes,resizable=yes,scrollbars=yes' );
	if ( hjalpOrd.focus ) hjalpOrd.focus();

	closeOpenOverlay = function() { if ( hjalpOrd.close ) hjalpOrd.close(); };

	return false;
}

function openFAQ( id, evnt, domain )
{
	stopBubble( evnt );
	
	if ( closeOpenOverlay ) closeOpenOverlay();
	var url = ((domain != null)? 'https://' + domain: '') + '/mux/page/hjalp/hjalp.html?id=' + id;
	var hjalpFAQ = window.open( url, 'nn_hjalpfaq', 'height=600,width=960,dependent=yes,resizable=yes,scrollbars=yes' );
	
	if ( hjalpFAQ ) hjalpFAQ.focus();
	closeOpenOverlay = function() { if ( hjalpFAQ.close ) hjalpFAQ.close(); };
	
	
	return false;
}

var supportNavWidget =
{
	openTabId: null,
	searchState: 0,
	numLogins: 0,
	context: '',
	handelsmetod: '',

	toggle: function( tabId, evnt )
	{
		if (evnt) stopBubble(evnt);

		if ( tabId == this.openTabId )
			this.close( tabId );
		else
			this.open( tabId );

		return false;
	},

	open: function( tabId, evnt )
	{
		removeMarket();
		if (evnt) stopBubble(evnt);

		if ( !tabId || tabId == this.openTabId )
			return false;

		if ( closeOpenOverlay )
			closeOpenOverlay();
		closeOpenOverlay = function() { supportNavWidget.close(); };

		switch ( tabId )
		{
			case 'logout':
				if ( this.numLogins == 0 )
				{
					logout();
					return;
				}

				hideSelectsIE();
				$("supportNavLogout").addClassName("foldout");
				$("loggaUt").show();
				break;

			case 'login':
				$("supportNavLogin").addClassName("foldout");
				$("loggaIn").show();
				$("loginForm").username.focus();
				break;

			case 'fastSeek':
				if (this.context == 'quicktrade') {
					hideSelectsIE("moreOrderConditions");
				} else {
					hideSelectsIE("container");
				}
				$("fastSeek").show();
				$("fastSeekInput").focus();
				break;
		}

		this.openTabId = tabId;

		return false;
	},

	close: function( tabId )
	{
		tabId = tabId || this.openTabId;		// default argument

		if ( !tabId )
			return false;

		switch ( tabId )
		{
			case 'logout':
				showSelectsIE();
				$("supportNavLogout").removeClassName("foldout");
				$("loggaUt").hide();
				break;

			case 'login':
				$("supportNavLogin").removeClassName("foldout");
				$("loggaIn").hide();
				break;

			case 'fastSeek':
				$("fastSeek").hide();
				$("fastSeekInput").value = '';
				showSelectsIE();

				if ( this.searchState > 0 )
				{
					this.searchState = 0;
					this.updateFastSeek( 'senaste.html' );
				}
				break;
		}

		this.openTabId = null;

		return false;
	},

	search: function(kopsalj, identifier, marketplace, vol)
	{
	
		removeMarket();
		if ( $("fastSeekInput").value.length == 0 && ( !identifier || !marketplace ) )
		{
			this.close();
			return;
		}

		kopsalj = kopsalj ? kopsalj : 'kop';

		this.open('fastSeek');

		if ( $("lastSearchesThrobber") )
			$("lastSearchesThrobber").show();

		this.searchState = 1;
		if ( this.context == 'quicktrade' )
		{
			var params = { search: $("fastSeekInput").value, kind: $("fastSeekKind").value, marknad: $("fast\SeekMarknad").value, kopsalj: kopsalj, produkt: 'quicktrade' };

			if ( identifier && marketplace )
			{
				params = { identifier: identifier, marketplace: marketplace, kopsalj: kopsalj, produkt: 'quicktrade', vol: vol };
			}
			
			new Ajax.Updater( 'snabbSokContent', '/mux/ajax/handla/sokresultat.html',
				{
					parameters: params,
					evalScripts: true
				}
			);
		}
		else{
			this.updateFastSeek( 'sokresultat.html', { search: $("fastSeekInput").value, kind: $("fastSeekKind").value, marknad: $("fastSeekMarknad").value } );

}
		return false;
	},

	fastStatus: function( instrumentid, marketid ,kind)
	{ 	
		if ( this.context == 'quicktrade' )
		{
			//refreshar
			new Ajax.Request( 
				'/mux/ajax/snabbsok/snabbstatus.html',
				{
					parameters: { instrumentid: instrumentid, marketid: marketid, kind:kind  },
					onSuccess: function() {
						supportNavWidget.updateFastSeek( 'senaste.html' );
					}
				}
			);
			this.populeraHandla( instrumentid, marketid, 'kop' );
			return false;
		}

		this.searchState = 2;
		this.updateFastSeek( 'snabbstatus.html', { instrumentid: instrumentid, marketid: marketid, kind: kind } );

		return false;
	},

	updateFastSeek: function( page, params )
	{	
		if ( this.searchState && this.openTabId != 'fastSeek' )
			this.open( 'fastSeek' );

			
		new Ajax.Updater( 'snabbSokContent', '/mux/ajax/snabbsok/' + page, { parameters: params || {}, evalScripts: true } );
		return false;
	},

	populeraHandla: function( identifier, marketplace, kopsalj )
	{
		if ( this.context != 'quicktrade' && this.handelsmetod == 'quicktrade' )
		{
			maybeQuicktrade( 'quicktrade', identifier, marketplace, kopsalj );
		}
		else
		{
			if ( !$("orderWrapper") || !populeraHandla )
				return true;

			populeraHandla( identifier, marketplace, kopsalj );
		}

		this.close();
		return false;
	},

	setNumLogins: function( num )
	{
		this.numLogins = num;
		return false;
	}
};


// Per
quickbarWidget = {
    reloadIDs: [],

    init: function() {
        if ($("quickbar") && $("quickbar_outer") && $("quickbar_outer").innerHTML.length > 1) {
            this.enableOverlay("qbar_index_link", "qbar_index_overlay", "index");
            this.enableOverlay("qbar_minaListor_link", "qbar_minaListor_overlay");
            this.enableOverlay("qbar_nyheter_link", "qbar_nyheter_overlay", "nyheter");

            if (this.visible()) this.startReloads();
        }
    },

    visible: function () {
        return $('quickbar').visible();
    },

    toggle: function() {
	if ($("quickbar_outer").innerHTML.length < 2) {
	    new Ajax.Updater( 'quickbar_outer', '/mux/ajax/quickbar/quickbar.html', { parameters: {}, evalScripts: true } );
	}
      toggleShow($('quickbarToggle'), 'uncurrent', 'current', 'quickbar');

      this.saveMode();

      if (this.visible()) {
        this.startReloads();
      } else {
        this.cancelReloads();
      }
    },

    startReloads: function() {
        this.cancelReloads();

        this.reloadIDs.push(this.setReload("valdaIndex", "valdaIndex", 20000));
        this.reloadIDs.push(this.setReload("depaInformation", "depaInformation", 60000));
    },

    cancelReloads: function() {
    	this.reloadIDs.each(function(int){clearInterval(int); });
        this.reloadIDs = [];
    },

    saveMode: function() {
        new Ajax.Request('/mux/ajax/setAjaxKey.html', {
          parameters: {
            longevity: 'persistent',
            key: 'quickbar_open',
            value: this.visible()
          }
        });
    },

    enableOverlay: function(linkId, overlayId, url) {
        var closeTimer;
        var displaying = false;

        Event.observe(linkId, 'mouseover', function() {
            if (url) _reload();
            _show();
        });

        Event.observe(linkId, 'mouseout', function() {
            _hide();
        });

        Event.observe(overlayId, 'mouseover', function() {
            _show();
        });

        Event.observe(overlayId, 'mouseout', function() {
            closeTimer = setTimeout(function() { _hide(); }, 200);
        });

        function _reload() {
            if ($(overlayId).style.visibility == "hidden") {
                new Ajax.Updater(overlayId, '/mux/ajax/quickbar/' + url + '.html', {
                    evalScripts: true
                });
            }
        }

        function _show() {
            clearTimeout(closeTimer);
            if (!displaying) {
                hideSelectsIE();
                _setStyles("visible", "foldout", "dropselection");
                displaying = true;
            }
        }

        function _hide() {
            if (displaying) {
                _setStyles("hidden", "dropselection", "foldout");
                showSelectsIE();
                displaying = false;
            }
        }

        function _setStyles(visibility, addClass, removeClass) {
            $(overlayId).style.visibility = visibility;
            $(linkId).removeClassName(removeClass);
            $(linkId).addClassName(addClass);
        }
    },

    setReload: function(containerId, url, interval) {
        var reloading = false;

        return setInterval(function() {
            if (!reloading) {
                reloading = true;
                new Ajax.Updater(containerId, '/mux/ajax/quickbar/' + url + '.html/auto', {
                    onComplete: function() { reloading = false; }
                });
            }
        }, interval);
    }
};

switchAccountWidget = {
    init: function(){
        this.enableOverlay("account_switcher", "account-overlay", "/mux/ajax/user/accountswitch.html");
    },
    
    closeOverlay: function(){
        $('account-overlay').style.display = "none";
        $('account_switcher').removeClassName("foldout");
        $('account_switcher').addClassName("dropselection");
    },

    enableOverlay: function(linkId, overlayId, url){
        var closeTimer, 
        	displaying = false,
        	isLoaded = false;
        
        Event.observe(linkId, 'mouseover', function(){
        	_show();
        	_load(url);
		_hideSearchResults();
        });
        
        Event.observe(linkId, 'mouseout', function(){
            _hide();
        });

        Event.observe(overlayId, 'mouseover', function(){
        	_show();
        });
        
        Event.observe(overlayId, 'mouseout', function(){
        	closeTimer = setTimeout(function() { _hide(); }, 200);
        });

        function _load(url){
        	if(!isLoaded){
	            new Ajax.Updater(overlayId, url, {
	                evalScripts: true
	            });
	            isLoaded = true;
        	}
        }
        
        function _show(){
            clearTimeout(closeTimer);
            if(!displaying){
            	_setStyles("block", "foldout", "dropselection");
            	displaying = true;
            }
        }
        
        function _hide(){
        	if(displaying){
        		_setStyles("none", "dropselection", "foldout");
        		showSelectsIE();
        		displaying = false;
        	}
        }
        
        function _setStyles(display, addClass, removeClass) {
            $(overlayId).style.display = display;
            $(linkId).removeClassName(removeClass);
            $(linkId).addClassName(addClass);
        }

        function _hideSearchResults() {
        	jQuery( '.ui-autocomplete' ).hide();
        }
    }
};

function removeMarket(){
	if ( $("fastSeekKind") && $("fastSeekMarknad")) {
		if ( $("fastSeekKind").value == 'FND') {
			document.getElementById ("fastSeekMarknad").style.visibility = 'hidden';
		}
		else {
			document.getElementById ("fastSeekMarknad").style.visibility = 'visible';
		}
	}
}

function isMobileDevice() {
	var useragent = navigator.userAgent.toLowerCase();
	
	if (useragent.indexOf("android") >= 0 || useragent.indexOf("iphone") >= 0 || useragent.indexOf("ipad") >= 0 || useragent.indexOf("ipod") >= 0) {
		return true;
	}
	
	return false;
}

function isIos() {
	return (navigator.userAgent.match(/(iPad|iPhone|iPod)/i) ? true : false);
}

function isAndroid() {
	var ua = navigator.userAgent.toLowerCase();
	return ua.indexOf("android") >= 0;
}

