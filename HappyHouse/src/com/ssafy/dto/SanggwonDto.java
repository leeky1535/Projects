package com.ssafy.dto;

public class SanggwonDto {
	private int no;
	private String name;
	private String branch;
	private String category;
	private String dongCode;
	private String address;
	private String buildingName;
	private String streetAddress;
	private String dong;
	private String floor;
	private String ho;
	private double lat;
	private double lng;

	public SanggwonDto(int no, String name, String branch, String category, String dongCode, String address,
			String buildingName, String streetAddress, String dong, String floor, String ho, double lat, double lng) {
		super();
		this.no = no;
		this.name = name;
		this.branch = branch;
		this.category = category;
		this.dongCode = dongCode;
		this.address = address;
		this.buildingName = buildingName;
		this.streetAddress = streetAddress;
		this.dong = dong;
		this.floor = floor;
		this.ho = ho;
		this.lat = lat;
		this.lng = lng;
	}

	public int getNo() {
		return no;
	}

	public void setNo(int no) {
		this.no = no;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getBranch() {
		return branch;
	}

	public void setBranch(String branch) {
		this.branch = branch;
	}

	public String getcategory() {
		return category;
	}

	public void setcategory(String category) {
		this.category = category;
	}

	public String getDongCode() {
		return dongCode;
	}

	public void setDongCode(String dongCode) {
		this.dongCode = dongCode;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getBuildingName() {
		return buildingName;
	}

	public void setBuildingName(String buildingName) {
		this.buildingName = buildingName;
	}

	public String getStreetAddress() {
		return streetAddress;
	}

	public void setStreetAddress(String streetAddress) {
		this.streetAddress = streetAddress;
	}

	public String getDong() {
		return dong;
	}

	public void setDong(String dong) {
		this.dong = dong;
	}

	public String getFloor() {
		return floor;
	}

	public void setFloor(String floor) {
		this.floor = floor;
	}

	public String getHo() {
		return ho;
	}

	public void setHo(String ho) {
		this.ho = ho;
	}

	public double getLat() {
		return lat;
	}

	public void setLat(double lat) {
		this.lat = lat;
	}

	public double getLng() {
		return lng;
	}

	public void setLng(double lng) {
		this.lng = lng;
	}

	@Override
	public String toString() {
		return "SangGwonDto [no=" + no + ", name=" + name + ", branch=" + branch + ", category=" + category
				+ ", dongCode=" + dongCode + ", address=" + address + ", buildingName=" + buildingName
				+ ", streetAddress=" + streetAddress + ", dong=" + dong + ", floor=" + floor + ", ho=" + ho + ", lat="
				+ lat + ", lng=" + lng + "]";
	}

}
