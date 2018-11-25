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

        this.state = {
            currentRoomID: undefined,
            newRoom: "",
        }
    }

    roomsDropdownOnChange(e) {
        this.setState({currentRoomID: e.target.textContent})

        options.forEach(option => {
            if (option.text === e.target.textContent) {
                this.setState({currentRoomID: option.value})
            }
        })
    }

    newRoomLabelOnChange(e) {
        this.setState({newRoom: e.target.value})
    }

    handleSubmit(event) {
        if (this.state.currentRoomID === undefined && this.state.newRoom === "") {
            alert("Please provide every neccesary data")
        } else {
            if (this.state.currentRoom) {
                this.props.history.push('/rooms/' + this.state.currentRoomID);
            } else {
                this.props.history.push('/rooms/' + this.state.newRoom);
            }
        }
    }


    handleData(data) {
        console.log(data);
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

    handleSubmit(event) {
        event.preventDefault();
        this.sendMessage(this.state.value);
    }

    sendMessage(message) {
        let x = JSON.stringify({
            'action':'GET_ROOMS'
        })
        x+= String.fromCharCode(1);
        console.log("wysylam"+x);
        this.refWebsocket.sendMessage(x);
    }
    

    render() {
        let websocketUrl = 'ws://localhost:8000'; // localStorage.getItem("ip");
        return (
            <div className={'BackgroundImg'}>
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