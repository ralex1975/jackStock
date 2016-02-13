var mainMenu = {
	showingMenuId: null,
	currentMenuId: "start",
	currentPageURL: window.location.href,
	currentPageMarked: false,

	setCurrentMenu: function(menuId) {
		this.currentMenuId = menuId;
	},

	toggle: function(menuId, evnt) {
		if (evnt) stopBubble(evnt);

		if (menuId == this.showingMenuId) {
			this.hide();
		} else {
			this.show(menuId);
		}
	},

	show: function(menuId) {
		if (closeOpenOverlay) {
			closeOpenOverlay();
			closeOpenOverlay = null;
		}

		this.hide();
		hideSelectsIE();

		this.showingMenuId = menuId;

		$(menuId).className = "foldout";
		$(this._portal()).show();

		if (menuId == this.currentMenuId) this.markCurrentPage();

		closeOpenOverlay = function() { mainMenu.hide(); };
	},

	hide: function() {
		if (!this.showingMenuId) return;

		$(this.showingMenuId).className = "";
		$(this._portal()).hide();

		showSelectsIE();

		if (this.showingMenuId == this.currentMenuId) this.markCurrentMenu();

		this.showingMenuId = null;
		closeOpenOverlay = null;
	},

	close: function() {
		this.hide();
	},

	
	markCurrentMenu: function() {
		if($(this.currentMenuId)){
			var regex=/index/; 
			if(!regex.test(window.location.href)){//only mark if not indexSite
				$(this.currentMenuId).className = "current";
			}
		}
	},
	

	markCurrentPage: function() {
        if (this.currentPageMarked) return;

		var links = $A(this._portal().getElementsByTagName("a"));

        for (var i = 0; i < links.length; i++) {
            if (links[i].href.charAt(links[i].href.length - 1) != "#") {
                if (this.currentPageURL == links[i].href) {
                    _mark(links[i]);
        			this.currentPageMarked = true;
                    break;
                }
            }
        }

		function _mark(linkObj) {
			var firstClass = $(linkObj).classNames().toArray().first();
			if (!firstClass) {
				$(linkObj).addClassName("current");
			} else {
				$(linkObj).removeClassName(firstClass);
				$(linkObj).addClassName(firstClass + "Current");
			}
		}
	},
	
	_portal: function() {
		return $("menyPortal" + this.showingMenuId.capitalize());
	}

};
