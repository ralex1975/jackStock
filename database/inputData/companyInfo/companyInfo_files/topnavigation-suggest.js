( function( $, window, document ) {
	$( function() {
		var topNavigationSuggestCleared = false,
			searchQuery = $( '#searchQuery' );
		
		if( searchQuery.length === 0) return;
		
		searchQuery
			.nordnetSearchSuggest()
			.on( 'focus', function() {
				var $this = $( this );

				if( !topNavigationSuggestCleared ) {
					$this.val( '' );
					topNavigationSuggestCleared = true;
				}

				searchQuery.data( 'nordnetSearchSuggest' ).search();
			});
		
		$( '#searchQueryForm' ).on( 'submit', function( event ) {
			submitSuggestForm();
			event.preventDefault();
		});
		
		$( document ).on( 'keypress', function( event ) {
			var fKey = 102,
				fCapitalKey = 70,
				forwardSlashKey = 47;
			
			if( ( event.which === fKey || event.which === fCapitalKey || event.which === forwardSlashKey ) 
					&& event.target.tagName !== 'INPUT' && event.target.tagName !== 'TEXTAREA' && !event.ctrlKey ) {     
				event.preventDefault();
				searchQuery.focus();             
			}	
		});

		$( document ).on( 'mouseup', function( event ) {
			var container = $( '.ui-autocomplete' );
		    if ( !searchQuery.is( event.target ) && !container.is( event.target )
		        && container.has( event.target ).length === 0 ) {
		        container.hide();
		    }
		});
	});
})( jQuery, window, document );

function submitSuggestForm( type, filter ) {
	var searchQuery = jQuery( '#searchQuery' );
	
	if( filter === 'LATEST' ) {
		searchQuery.data( 'nordnetSearchSuggest' ).$elem.autocomplete( 'close' );
		return;
	}

	var onSearchPage = window.location.href.indexOf('/search/') !== -1;

	if( onSearchPage ) {
            jQuery( '#search-input' ).focus();
    }
    
	window.location.href = '/search/#query:' + jQuery( '#searchQuery' ).val() 
		+ ( type !== undefined ? '/type:' + type : '/type:instrument' )
		+ ( filter !== undefined ? '/instrument_group_type:' + filter : '' );
	if( onSearchPage ) window.location.reload(true);
}
