//
//  ContentView.swift
//  MyOwnCardial
//
//  Created by Collin Trump on 4/11/23.
//

import SwiftUI

struct ContentView: View {
    
    @State var counter: Int = 0
    
    @State private var showWebView = true
    var body: some View {
        
        VStack {
            WebView(url: URL(string: "https://myowncardial.web.app/")!)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
