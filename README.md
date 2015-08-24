WebViewRS
==================

This plugin contains a widget with tabbed QWebView - like a tabbed browser  
The WebViews have access to some RS Features like file search, or sending data to peers running the same plugin.  

##Compile & run

Depends on qt5.4+

this plugin should be built in the retroshare plugins directory, along the lines of:

    cd myretrosharedir/plugins
    git clone <this repo>
    cd <new folder>
    qmake
    make
    cp *so* ~/.retroshare/extensions6/

Then reboot retroshare, it will ask if you want to accept the plugin.

