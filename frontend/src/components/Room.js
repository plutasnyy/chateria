import React from 'react'
import './../css/HomePage.css'
import './../css/Room.css'
import {Button, Container, Form} from "semantic-ui-react";
import Websocket from './Websocket';

var roomData = null;

class Room extends React.Component {


    constructor(props) {
        super(props);
        this.state = {
            value: "",
            mes: []
        };
        this.onOpen = this.onOpen.bind(this);
        this.handleData = this.handleData.bind(this);
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.sendMessage = this.sendMessage.bind(this);
        this.goHome = this.goHome.bind(this);
    }

    handleData(data) {
        console.log(data);
    }

    onOpen() {
        console.log("Open ws");
        let addToRoomMessage = JSON.stringify({
            'action': 'ADD_TO_ROOM',
            'room': this.props.match.params.roomID,
            'nick': localStorage.getItem('nick'),
        });
        this.sendMessage(addToRoomMessage);

    }

    onClose() {
        console.log("Close ws");
    }

    handleChange(event) {
        this.setState({value: event.target.value});
    }

    handleSubmit(event) {
        event.preventDefault();
        let sendMessage = JSON.stringify({
            'action':'MESSAGE',
            'nick': localStorage.getItem('nick'),
            'room': this.props.match.params.roomID,
            'message':this.state.value,
        });
        this.sendMessage(sendMessage);
    }

    goHome() {
        this.sendMessage(JSON.stringify({
            'action': 'EXIT_ROOM',
            'nick': localStorage.getItem('nick'),
            'room': this.props.match.params.roomID,
        }));
        this.props.history.push('/');
    }

    sendMessage(message) {
        message += String.fromCharCode(1);
        console.log("SEND: " + message);
        this.refWebsocket.sendMessage(message);
    }

    render() {
        let websocketUrl = 'ws://localhost:8000/ws/chat/' + this.props.match.params.roomID + '/';
        return (
            <div className={'BackgroundImg'}>
                <div className={'RoomContainer'}>
                    <h3> Hello {localStorage.getItem('nick')} in <span id='roomHeader'>{this.props.match.params.roomID}</span></h3>

                    <div id="chat-log" style={{'overflow': 'auto'}}>
                        {this.state.mes.map(function (item, i) {
                            return <div><strong>item author</strong>: item content </div>
                        })}
                    </div>

                    <Form onSubmit={this.handleSubmit} style={{'bottom': '0px'}}>
                        <Form.Field>
                            <label>
                                Your message:
                                <input type="text" value={this.state.value} onChange={this.handleChange}/>
                            </label>
                        </Form.Field>
                        <Button type='submit' style={{'float': 'right'}}>Send message</Button>
                    </Form>
                    <Button onClick={this.goHome}>Back to home</Button>
                </div>
                <Websocket url={websocketUrl} onMessage={this.handleData} onOpen={this.onOpen} onClose={this.onClose}
                           ref={Websocket => {
                               this.refWebsocket = Websocket;
                           }}/>

            </div>
        )
    }
}

export default Room;