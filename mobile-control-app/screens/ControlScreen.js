import React, { useState, useEffect } from 'react';
import { 
  View, 
  Text, 
  StyleSheet, 
  TouchableOpacity,
  ScrollView,
  Alert,
  PanResponder,
  Animated
} from 'react-native';
import { Ionicons } from '@expo/vector-icons';

const ControlScreen = ({ navigation }) => {
  const [sensorData, setSensorData] = useState({
    right: 0,
    front: 0,
    back: 0
  });
  const [isConnected, setIsConnected] = useState(false);
  const [ipAddress, setIpAddress] = useState('192.168.1.100'); // Default IP
  const [joystickPosition, setJoystickPosition] = useState({ x: 0, y: 0 });

  // Function to send command to ESP32
  const sendCommand = async (command) => {
    try {
      const response = await fetch(`http://${ipAddress}/${command}`);
      if (!response.ok) {
        throw new Error('Network response was not ok');
      }
    } catch (error) {
      Alert.alert('Error', 'Failed to send command to robot');
    }
  };

  // Handle touch movement
  const handleTouchMove = (event) => {
    const { locationX, locationY } = event.nativeEvent;
    const centerX = 75; // Half of joystick width
    const centerY = 75; // Half of joystick height
    
    // Calculate distance from center
    const deltaX = locationX - centerX;
    const deltaY = locationY - centerY;
    
    // Normalize values to -1 to 1
    const normalizedX = Math.max(-1, Math.min(1, deltaX / centerX));
    const normalizedY = Math.max(-1, Math.min(1, deltaY / centerY));
    
    setJoystickPosition({ x: normalizedX, y: normalizedY });
    
    // Convert joystick coordinates to commands
    if (Math.abs(normalizedX) > 0.5 || Math.abs(normalizedY) > 0.5) {
      if (Math.abs(normalizedX) > Math.abs(normalizedY)) {
        // Horizontal movement
        if (normalizedX > 0) {
          sendCommand('R');
        } else {
          sendCommand('L');
        }
      } else {
        // Vertical movement
        if (normalizedY > 0) {
          sendCommand('B');
        } else {
          sendCommand('F');
        }
      }
    } else {
      sendCommand('S');
    }
  };

  const handleTouchEnd = () => {
    setJoystickPosition({ x: 0, y: 0 });
    sendCommand('S');
  };

  // Update sensor data periodically
  useEffect(() => {
    const fetchSensorData = async () => {
      try {
        const response = await fetch(`http://${ipAddress}/sensors`);
        const data = await response.json();
        setSensorData(data);
        setIsConnected(true);
      } catch (error) {
        console.error('Failed to fetch sensor data:', error);
        setIsConnected(false);
      }
    };

    const interval = setInterval(fetchSensorData, 1000);
    return () => clearInterval(interval);
  }, [ipAddress]);

  return (
    <ScrollView style={styles.container}>
      <View style={styles.connectionStatus}>
        <Text style={[styles.statusText, { color: isConnected ? '#34C759' : '#FF3B30' }]}>
          Status: {isConnected ? 'Connected' : 'Disconnected'}
        </Text>
      </View>

      <View style={styles.joystickContainer}>
        <View 
          style={styles.joystickBase}
          onTouchMove={handleTouchMove}
          onTouchEnd={handleTouchEnd}
        >
          <View 
            style={[
              styles.joystick,
              { 
                transform: [
                  { translateX: joystickPosition.x * 50 },
                  { translateY: joystickPosition.y * 50 }
                ]
              }
            ]} 
          />
        </View>
      </View>

      <View style={styles.buttonContainer}>
        <TouchableOpacity 
          style={styles.button} 
          onPress={() => sendCommand('F')}
        >
          <Ionicons name="arrow-up" size={24} color="white" />
        </TouchableOpacity>
        <View style={styles.horizontalButtons}>
          <TouchableOpacity 
            style={styles.button} 
            onPress={() => sendCommand('L')}
          >
            <Ionicons name="arrow-back" size={24} color="white" />
          </TouchableOpacity>
          <TouchableOpacity 
            style={[styles.button, styles.stopButton]} 
            onPress={() => sendCommand('S')}
          >
            <Ionicons name="stop" size={24} color="white" />
          </TouchableOpacity>
          <TouchableOpacity 
            style={styles.button} 
            onPress={() => sendCommand('R')}
          >
            <Ionicons name="arrow-forward" size={24} color="white" />
          </TouchableOpacity>
        </View>
        <TouchableOpacity 
          style={styles.button} 
          onPress={() => sendCommand('B')}
        >
          <Ionicons name="arrow-down" size={24} color="white" />
        </TouchableOpacity>
      </View>

      <View style={styles.sensorContainer}>
        <Text style={styles.sensorTitle}>Sensor Readings</Text>
        <View style={styles.sensorRow}>
          <Text style={styles.sensorLabel}>Right:</Text>
          <Text style={styles.sensorValue}>{sensorData.right} cm</Text>
        </View>
        <View style={styles.sensorRow}>
          <Text style={styles.sensorLabel}>Front:</Text>
          <Text style={styles.sensorValue}>{sensorData.front} cm</Text>
        </View>
        <View style={styles.sensorRow}>
          <Text style={styles.sensorLabel}>Back:</Text>
          <Text style={styles.sensorValue}>{sensorData.back} cm</Text>
        </View>
      </View>
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  connectionStatus: {
    padding: 10,
    backgroundColor: '#fff',
    marginBottom: 10,
  },
  statusText: {
    fontSize: 16,
    textAlign: 'center',
  },
  joystickContainer: {
    height: 200,
    justifyContent: 'center',
    alignItems: 'center',
  },
  joystickBase: {
    width: 150,
    height: 150,
    borderRadius: 75,
    backgroundColor: '#e0e0e0',
    justifyContent: 'center',
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 3.84,
    elevation: 5,
  },
  joystick: {
    width: 50,
    height: 50,
    borderRadius: 25,
    backgroundColor: '#007AFF',
    position: 'absolute',
  },
  buttonContainer: {
    alignItems: 'center',
    marginVertical: 20,
  },
  horizontalButtons: {
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
  },
  button: {
    width: 60,
    height: 60,
    backgroundColor: '#007AFF',
    borderRadius: 30,
    justifyContent: 'center',
    alignItems: 'center',
    margin: 5,
  },
  stopButton: {
    backgroundColor: '#FF3B30',
  },
  sensorContainer: {
    backgroundColor: '#fff',
    padding: 15,
    margin: 10,
    borderRadius: 10,
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 3.84,
    elevation: 5,
  },
  sensorTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 10,
  },
  sensorRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginVertical: 5,
  },
  sensorLabel: {
    fontSize: 16,
  },
  sensorValue: {
    fontSize: 16,
    fontWeight: 'bold',
  },
});

export default ControlScreen; 