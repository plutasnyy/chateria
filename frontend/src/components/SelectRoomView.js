import React from 'react'
import {Button, Form, Header, Dropdown} from 'semantic-ui-react'
import './../css/HomePage.css'
import {create} from 'axios';
import Websocket from "./Websocket";

var options = []

class SelectRoomView extends React.Component {

    constructor(props) {
        super(props);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.roomsDropdownOnChange = this.roomsDropdownOnChange.bind(this);
        this.newRoomLabelOnChange = this.newRoomLabelOnChange.bind(this);

        this.handleData = this.handleData.bind(this);
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.sendMessage = this.sendMessage.bind(this);
        this.getRooms = this.getRooms.bind(this);

        this.state = {
            currentRoomName: undefined,
            newRoom: "",
        }
    }

    roomsDropdownOnChange(e) {
        this.setState({currentRoomName: e.target.textContent})

        options.forEach(option => {
            if (option.text === e.target.textContent) {
                this.setState({currentRoomName: option.value})
            }
        })
    }

    newRoomLabelOnChange(e) {
        this.setState({newRoom: e.target.value})
    }

    handleSubmit(event) {
        if (this.state.currentRoomName === undefined && this.state.newRoom === "") {
            alert("Please provide every neccesary data")
        } else {
            if (this.state.currentRoom) {
                this.props.history.push('/rooms/' + this.state.currentRoomName + '/' + localStorage.getItem('nick'));
            } else {
                let roomName = this.state.newRoom.split(' ').join('_');
                roomName = roomName.split('/').join('_');
                this.props.history.push('/rooms/' + roomName + '/' + localStorage.getItem('nick'));
            }
        }
    }


    handleData(data) {
        console.log(data);
    }

    getRooms() {
        let getRoomsMessage = JSON.stringify({
            'action': 'GET_ROOMS'
        })
        this.sendMessage(getRoomsMessage);
        return false;
    }


    onOpen() {
        console.log("Open ws");
    }

    onClose() {
        console.log("Close ws");
    }

    handleChange(event) {
        this.setState({value: event.target.value});
    }

    sendMessage(message) {
        message += String.fromCharCode(1);
        console.log("SEND: " + message);
        this.refWebsocket.sendMessage(message);
    }


    render() {
        let websocketUrl = 'ws://localhost:8000'; // localStorage.getItem("ip");
        return (
            <div className={'BackgroundImg'}>
                <Button onClick={this.getRooms}>Get Rooms</Button>
                <Form className={'NickNameFormContainer'} onSubmit={this.handleSubmit}>
                    <Header as='h1' textAlign={'center'}>Hi!</Header>
                    <div className={'NickNameForm'}>
                        <span>
                            <strong>Select a room to go in:{' '} </strong>
                            <Dropdown inline options={options} onChange={this.roomsDropdownOnChange}/>
                        </span>
                        <Form.Field style={{'marginTop': '16px'}}>
                            <label> Or create new </label>
                            <input placeholder={'Please provide room name'} onChange={this.newRoomLabelOnChange}/>
                        </Form.Field>
                        <Button type='submit'>Next</Button>
                    </div>
                </Form>
                <Websocket url={websocketUrl} onMessage={this.handleData} onOpen={this.onOpen} onClose={this.onClose}
                           ref={Websocket => {
                               this.refWebsocket = Websocket;
                           }}/>
            </div>

        )
    }
}

export default SelectRoomView