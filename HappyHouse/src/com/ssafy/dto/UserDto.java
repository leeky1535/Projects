package com.ssafy.dto;

public class UserDto {
	private String userId;
	private String password;
	private String name;
	private String email;
	private String phone;
	private String address;
	private String addressDetail;
	private String favoriteDongCode;

	public UserDto(String userId, String password, String name, String email, String phone, String address,
			String addressDetail, String favoriteDongCode) {
		super();
		this.userId = userId;
		this.password = password;
		this.name = name;
		this.email = email;
		this.phone = phone;
		this.address = address;
		this.addressDetail = addressDetail;
		this.favoriteDongCode = favoriteDongCode;
	}

	public String getUserId() {
		return userId;
	}

	public void setUserId(String userId) {
		this.userId = userId;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public String getAddress() {
		return address;
	}

	public void setDongCode(String address) {
		this.address = address;
	}

	public String getAddressDetail() {
		return addressDetail;
	}

	public void setAddressDetail(String addressDetail) {
		this.addressDetail = addressDetail;
	}

	public String getFavoriteDongCode() {
		return favoriteDongCode;
	}

	public void setFavoriteDongCode(String favoriteDongCode) {
		this.favoriteDongCode = favoriteDongCode;
	}

	@Override
	public String toString() {
		return "UserDto [userId=" + userId + ", password=" + password + ", name=" + name + ", email=" + email
				+ ", phone=" + phone + ", address=" + address + ", addressDetail=" + addressDetail
				+ ", favoriteDongCode=" + favoriteDongCode + "]";
	}
	
}
