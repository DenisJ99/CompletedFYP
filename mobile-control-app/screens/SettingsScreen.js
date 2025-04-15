import React, { useState } from 'react';
import {
  View,
  Text,
  StyleSheet,
  TextInput,
  TouchableOpacity,
  Alert,
  ScrollView
} from 'react-native';

const SettingsScreen = () => {
  const [ipAddress, setIpAddress] = useState('192.168.1.100');
  const [wifiSSID, setWifiSSID] = useState('DJ');
  const [wifiPassword, setWifiPassword] = useState('Johnson1');

  const handleSave = () => {
    // Here you would typically save these settings to AsyncStorage
    Alert.alert('Success', 'Settings saved successfully');
  };

  const handleTestConnection = async () => {
    try {
      const response = await fetch(`http://${ipAddress}/test`);
      if (response.ok) {
        Alert.alert('Success', 'Connected to robot successfully');
      } else {
        throw new Error('Connection failed');
      }
    } catch (error) {
      Alert.alert('Error', 'Failed to connect to robot');
    }
  };

  return (
    <ScrollView style={styles.container}>
      <View style={styles.section}>
        <Text style={styles.sectionTitle}>Robot Connection</Text>
        <View style={styles.inputContainer}>
          <Text style={styles.label}>IP Address</Text>
          <TextInput
            style={styles.input}
            value={ipAddress}
            onChangeText={setIpAddress}
            placeholder="Enter IP Address"
            keyboardType="numeric"
          />
        </View>
        <TouchableOpacity 
          style={styles.button}
          onPress={handleTestConnection}
        >
          <Text style={styles.buttonText}>Test Connection</Text>
        </TouchableOpacity>
      </View>

      <View style={styles.section}>
        <Text style={styles.sectionTitle}>WiFi Settings</Text>
        <View style={styles.inputContainer}>
          <Text style={styles.label}>SSID</Text>
          <TextInput
            style={styles.input}
            value={wifiSSID}
            onChangeText={setWifiSSID}
            placeholder="Enter WiFi SSID"
          />
        </View>
        <View style={styles.inputContainer}>
          <Text style={styles.label}>Password</Text>
          <TextInput
            style={styles.input}
            value={wifiPassword}
            onChangeText={setWifiPassword}
            placeholder="Enter WiFi Password"
            secureTextEntry
          />
        </View>
      </View>

      <TouchableOpacity 
        style={[styles.button, styles.saveButton]}
        onPress={handleSave}
      >
        <Text style={styles.buttonText}>Save Settings</Text>
      </TouchableOpacity>
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
    padding: 15,
  },
  section: {
    backgroundColor: '#fff',
    padding: 15,
    borderRadius: 10,
    marginBottom: 20,
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 3.84,
    elevation: 5,
  },
  sectionTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 15,
  },
  inputContainer: {
    marginBottom: 15,
  },
  label: {
    fontSize: 16,
    marginBottom: 5,
  },
  input: {
    borderWidth: 1,
    borderColor: '#ddd',
    borderRadius: 5,
    padding: 10,
    fontSize: 16,
  },
  button: {
    backgroundColor: '#007AFF',
    padding: 15,
    borderRadius: 5,
    alignItems: 'center',
  },
  saveButton: {
    backgroundColor: '#34C759',
    marginTop: 10,
  },
  buttonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: 'bold',
  },
});

export default SettingsScreen; 