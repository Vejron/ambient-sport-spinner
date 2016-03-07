Thing = new Mongo.Collection("thing");

if (Meteor.isClient) {
    // counter starts at 0
    Session.setDefault('counter', 0);

    Template.body.helpers({
        counter: function() {
            return Session.get('counter');
        }
    });

    Template.run.events({
        'click button': function() {
            // increment the counter when button is clicked
            Session.set('counter', Session.get('counter') + 1);
            // Make the thing spin
            Meteor.call('spin', {frames: 6, delay: 200});
        }
    });
    Template.run1.events({
        'click button': function() {
            // increment the counter when button is clicked
            Session.set('counter', Session.get('counter') + 1);
            // Make the thing spin
            Meteor.call('spin', {frames: 12, delay: 60});
        }
    });
    Template.run2.events({
        'click button': function() {
            // increment the counter when button is clicked
            Session.set('counter', Session.get('counter') + 1);
            // Make the thing spin
            Meteor.call('spin', {frames: 24, delay: 100});
        }
    });
}

if (Meteor.isServer) {
    Meteor.startup(function() {
        // code to run on server at startup

        //connect to broker and subscribe to status message
        Thing.mqttConnect('mqtt://test.mosquitto.org', ['fredrik40/beat/'], {
            insert: false,
            raw: false
        });
    });
    
    Meteor.methods({
    'spin': function(doc) {
      Thing.insert({ topic: 'fredrik40/run/', message: JSON.stringify(doc), broadcast: true });
      console.log(Thing.find().fetch());
    },
    'other': function() {
      // add something else
    }
});
}