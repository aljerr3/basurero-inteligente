import logo from "./logo.svg";
import "./App.css";
import React from "react";
import Sidebar from "./components/Sidebar";
import InputChat from "./components/InputChat";
import Disclaimer from "./components/Disclaimer";

function App() {
  return (
    <div>
      <Sidebar/> 
      <Disclaimer/>
      <div className="components">
        <div className="inputBar">
          <InputChat />
          <button className="sendButton">S</button>
        </div>
      </div>
    </div>
  );
}

export default App;