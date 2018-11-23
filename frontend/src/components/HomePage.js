import React from 'react'
import {Button, Form, Header} from 'semantic-ui-react'
import './../css/HomePage.css'


class HomePage extends React.Component {

    constructor(props) {
        super(props);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.nickLabelOnChange = this.nickLabelOnChange.bind(this);
        this.ipOnChange = this.ipOnChange.bind(this);

        this.state = {
            nick: "",
            ip: "127.0.0.1:8000",
        }
    }

    ipOnChange(e) {
        this.setState({ip: e.target.value})
    }

    nickLabelOnChange(e) {
        this.setState({nick: e.target.value})
    }

    handleSubmit(event) {
        if (this.state.nick === "" || this.state.ip === "") {
            alert("Please provide every neccesary data")
        } else {
            localStorage.setItem("nick", this.state.nick);
            localStorage.setItem("ip", this.state.ip);
            this.props.history.push('/select');
        }
    }

    render() {
        return (
            <div className={'BackgroundImg'}>
                <Form className={'NickNameFormContainer'} onSubmit={this.handleSubmit}>
                    <Header as='h1' textAlign={'center'}>Hi!</Header>
                    <div className={'NickNameForm'}>
                        <Form.Field>
                            <label> Register your name </label>
                            <input placeholder={'Please provide your username'} onChange={this.nickLabelOnChange}/>
                        </Form.Field>
                        <Form.Field style={{'marginTop': '16px'}}>
                            <label> IP:PORT </label>
                            <input placeholder={'eg. 127.0.0.1:1234'} onChange={this.ipOnChange}/>
                        </Form.Field>
                        <Button type='submit'>Next</Button>
                    </div>
                </Form>
            </div>
        )
    }
}

export default HomePage