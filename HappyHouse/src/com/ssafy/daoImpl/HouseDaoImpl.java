package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.HouseDao;
import com.ssafy.dto.HouseDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class HouseDaoImpl implements HouseDao {
	private final String DRIVER = "com.mysql.cj.jdbc.Driver";
	private final String URL = "jdbc:mysql://127.0.0.1:3306/happyhouse?serverTimezone=UTC&useUniCode=yes&characterEncoding=UTF-8";
	private final String DB_ID = "ssafy";
	private final String DB_PASS = "ssafy";

	private static HouseDao houseDao;

	private HouseDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static HouseDao getHouseDao() {
		if (houseDao == null)
			houseDao = new HouseDaoImpl();
		return houseDao;
	}

	@Override
	public void register(HouseDto HouseDto) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			StringBuilder sql = new StringBuilder();
			sql.append("insert into houseinfo (no, dong, AptName, code, buildYear, jibun, lat, lng, img)\n"
					+ "values (?,?,?,?,?,?,?,?,?)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setInt(1, HouseDto.getNo());
			pstmt.setString(2, HouseDto.getDong());
			pstmt.setString(3, HouseDto.getAptName());
			pstmt.setString(4, HouseDto.getCode());
			pstmt.setString(5, HouseDto.getBuildYear());
			pstmt.setString(6, HouseDto.getJibun());
			pstmt.setString(7, HouseDto.getLat());
			pstmt.setString(8, HouseDto.getLng());
			pstmt.setString(9, HouseDto.getImg());

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 삽입성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public HouseDto searchById(int houseNo) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		HouseDto houseDto = null;

		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, buildYear, jibun, lat, lng, img " + "where no = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setInt(1, houseNo);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				houseDto = new HouseDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
						rs.getString("code"), rs.getString("buildYear"), rs.getString("jibun"), rs.getString("lat"),
						rs.getString("lng"), rs.getString("img"));
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDto;
	}

	@Override
	public List<HouseDto> searchByDong(String houseDong) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HouseDto> houseDtoList = new ArrayList<HouseDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, buildYear, jibun, lat, lng, img " + "from houseinfo "
					+ "where dong = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, houseDong);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDto houseDto = new HouseDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
						rs.getString("code"), rs.getString("buildYear"), rs.getString("jibun"), rs.getString("lat"),
						rs.getString("lng"), rs.getString("img"));
				houseDtoList.add(houseDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDtoList;
	}

	@Override
	public List<HouseDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HouseDto> houseDtoList = new ArrayList<HouseDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, buildYear, jibun, lat, lng, img " + "from houseinfo";
			pstmt = conn.prepareStatement(sql);

			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDto houseDto = new HouseDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
						rs.getString("code"), rs.getString("buildYear"), rs.getString("jibun"), rs.getString("lat"),
						rs.getString("lng"), rs.getString("img"));
				houseDtoList.add(houseDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDtoList;
	}

	@Override
	public void deleteHouse(String houseNo) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			StringBuilder sql = new StringBuilder();
			sql.append("delete from houseinfo\n" + "where no = ?");
			pstmt = conn.prepareStatement(sql.toString());

			pstmt.setString(1, houseNo);
			pstmt.executeUpdate();
			System.out.println("삭제 성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

}
