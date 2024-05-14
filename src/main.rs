use core::fmt;
use std::{collections::HashMap, fs::File, io::Read, path::PathBuf};

#[derive(Debug)]
pub enum Status {
    Charging,
    Discharging,
    NotCharging,
}

#[derive(Debug)]
pub enum Capacity {
    Full,
    Normal,
    Low,
    Critical,
    Empty,
}

impl From<String> for Status {
    fn from(value: String) -> Self {
        match value.as_str() {
            "Not Charging" => Self::NotCharging,
            "Discharging" => Self::Discharging,
            "Charging" => Self::Charging,
            _ => Self::Discharging,
        }
    }
}

impl From<Status> for String {
    fn from(val: Status) -> Self {
        match val {
            Status::NotCharging => "Not Charging",
            Status::Discharging => "Discharging",
            Status::Charging => "Charging",
        }
        .to_owned()
    }
}

#[derive(Debug)]
pub struct InvalidBatteryError {
    id: u8,
}

impl InvalidBatteryError {
    pub fn new(id: u8) -> Self {
        Self { id }
    }
}

impl fmt::Display for InvalidBatteryError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Inavalid battery \"BAT{}\"!", self.id)
    }
}

impl std::error::Error for InvalidBatteryError {
    fn cause(&self) -> Option<&dyn std::error::Error> {
        None
    }

    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        None
    }
}

#[derive(Debug)]
#[allow(dead_code)]
pub struct Battery {
    id: u8,
    status: Status,
    present: bool,
    technology: Box<str>,
    cycle_count: u32,
    voltage_min_design: u64,
    voltage_now: u64,
    power_now: u64,
    design_capacity: u64,
    capacity_full: u64,
    capacity_now: u64,
    capacity_percent: u8,
    capacity_level: Capacity,
    model_name: Box<str>,
    manufacturer: Box<str>,
    serial: Box<str>,
}

impl Battery {
    pub fn new(id: u8) -> Result<Self, InvalidBatteryError> {
        let uevent_path: PathBuf = format!("/sys/class/power_supply/BAT{}/uevent", id).into();
        if !uevent_path.exists() {
            return Err(InvalidBatteryError::new(id));
        }

        let mut file = File::open(&uevent_path).unwrap();
        let mut file_content = String::new();
        file.read_to_string(&mut file_content).unwrap();

        let mut map: HashMap<String, String> = HashMap::new();
        file_content
            .split('\n')
            .map(|val| {
                let slc = val.split('=').collect::<Vec<&str>>();
                if !slc[0].is_empty() {
                    [slc[0][13..].to_owned(), slc[1].to_owned()]
                } else {
                    [slc[0].to_owned(), "".to_owned()]
                }
            })
            .for_each(|val| {
                if !val[0].is_empty() {
                    _ = map.insert(val[0].to_owned(), val[1].to_owned())
                }
            });

        Ok(Self {
            id,
            status: map["STATUS"].clone().into(),
            present: map["PRESENT"] == "1",
            technology: map["TECHNOLOGY"].clone().into(),
            cycle_count: map["CYCLE_COUNT"].parse::<u32>().unwrap(),
            voltage_min_design: map["VOLTAGE_MIN_DESIGN"].parse::<u64>().unwrap(),
            voltage_now: map["VOLTAGE_NOW"].parse::<u64>().unwrap(),
            power_now: map["POWER_NOW"].parse::<u64>().unwrap(),
            design_capacity: map["ENERGY_FULL_DESIGN"].parse::<u64>().unwrap(),
            capacity_full: map["ENERGY_FULL"].parse::<u64>().unwrap(),
            capacity_now: map["ENERGY_NOW"].parse::<u64>().unwrap(),
            capacity_percent: map["CAPACITY"].parse::<u8>().unwrap(),
            capacity_level: Capacity::Normal, // FIXME:
            model_name: map["MODEL_NAME"].clone().into(),
            manufacturer: map["MANUFACTURER"].clone().into(),
            serial: map["SERIAL_NUMBER"].clone().into(),
        })
    }
}

fn main() {
    println!("{:?}", Battery::new(0).unwrap());
}
