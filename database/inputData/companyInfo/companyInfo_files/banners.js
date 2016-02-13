/* Banner - "namespace" med subrutiner som hanterar olika aspekter av bannervisning. */

var Banner = (window.Banner)? window.Banner: {

	/* _visaDoljDropdowns(x) visar (om x sannt) eller döljer (om x falskt) alla select-fält på sidan. */

	_visaDoljDropdowns: function (visa) {
		return false;
		/*
		 * Robin try to remove this since he thinks it´s an IE6 thing.
		 var selects = $$('select');
		for (var i = 0; i < selects.length; i++) {
			selects[i].style.visibility = visa? 'visible': 'hidden';
			
		}*/
	},
	
	//ETF center mod Robin
	_hideDialog_news: function (visa)
	{
		if($('dialog_news')){
			$('dialog_news').hide();
		}
	},

	/* foljLank(id, url) öppnar angiven URL och ökar klickstatistiken för angiven banner med ett. Urlen öppnas i samma fönster om den innehåller /mux/web/, annars i nytt fönster. */

	foljLank: function (bannerID, url) {
		function oppna()
		{
			window.location = url;
		}
	
		new Ajax.Request('/mux/ajax/common/banners/raknaUppAntalKlick.html', {parameters: {bannerid: bannerID}, onComplete: oppna});
	},

    /* Instead of onclick for banners, register an click-listener on the banner. */
        registerClickListener : function (elementID, bannerID, url) {
	    Event.observe(
		$(elementID),
		'click',
		function () {
		    Banner.foljLank(bannerID, url);
		}
	    );
        },

	/* visaDoljHuvuderbjudande([visa]) visar (om dold) eller döljer (om visas) popupen för huvuderbjudandet. Om 'visa' anges (true/false) så avgör istället denna flagga ifall popupen ska visas eller döljas. */

	visaDoljHuvuderbjudande: function (visa) {
		function oppnaPopup()
		{
			Banner._visaDoljDropdowns(false);
			$('bannerOne').show();
		}

		if ($('bannerBox').visible()) {
			Banner.visaDoljAllaErbjudanden()
		}
		if ($('bannerOne').visible() || (visa != null) && !visa) {
			Banner._visaDoljDropdowns(true);
			$('bannerOne').hide();
		} else {
			new Ajax.Updater(
				'bannerOne',
				'/mux/ajax/common/banners/erbjudanden/huvuderbjudande.html',
				{onComplete: oppnaPopup}
			);
		}
	},
	
	/* visaDoljAllaErbjudanden([visa]) visar (om dold) eller döljer (om visas) rullgardinsmenyn som visar alla erbjudanden. Om 'visa' anges (true/false) så avgör istället denna flagga ifall menyn ska visas eller döljas. */
	
	visaDoljAllaErbjudanden: function (visa) {
		function oppnaPopup()
		{
			Banner._visaDoljDropdowns(false);
			$('bannerBox').show();
		}
		
		if ($('bannerOne').visible()) {
			Banner.visaDoljHuvuderbjudande()
		}
		// toggleShow($('bannerBoxLink'), 'dropselection', 'dropactive', 'bannerBox');
		if ($('bannerBox').visible() || (visa != null) && !visa) {
			Banner._visaDoljDropdowns(true);
			$('bannerBoxLink').removeClassName('dropactive');
			$('bannerBoxLink').addClassName('dropselection');
			$('bannerBox').hide();
		} else {
			$('bannerBoxLink').removeClassName('dropselection');
			$('bannerBoxLink').addClassName('dropactive');
			new Ajax.Updater(
				'bannerBox',
				'/mux/ajax/common/banners/erbjudanden/allaErbjudanden.html',
				{onComplete: oppnaPopup}
			);
		}
	},

	/* visaBliKundLoggaIn() visar logga-in-bli-kund-popupen. */

	visaBliKundLoggaIn: function () {
		function oppnaPopup()
		{
			Banner._visaDoljDropdowns(false);
			Banner._hideDialog_news(false);
		}

		new Ajax.Updater(
			'bliKundLoggaIn',
			'/mux/ajax/common/banners/bliKundLoggaIn/bliKundLoggaIn.html',
			{parameters: {popup: 1},evalScripts:true, onComplete: oppnaPopup}
		);
	},

	/* doljBliKundLoggaIn() stänger logga-in-bli-kund-popupen. */

	doljBliKundLoggaIn: function () {
		Banner._visaDoljDropdowns(true);
		if($("dimmerbox")){
			$("dimmerbox").hide();
		}
		$('bliKundLoggaIn').hide();
	}
}
