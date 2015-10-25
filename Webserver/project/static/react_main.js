//ReactDOM.render(<h1>Hello, world!</h1>, document.getElementById('mount_react'));

var SearchForm = React.createClass({
    handleSubmit: function(e) {
        e.preventDefault();
        var searchText2 = this.refs.searchText.value.trim();
        this.props.onSearchSubmit({searchText: searchText2});
        return;
    },
    render: function() {
        return (
            <div>
                <form className="searchForm" onSubmit={this.handleSubmit}>
                    <div className="input-group">
                        <input type="text" className="form-control" placeholder="Machine Learning" ref="searchText" />
                        <div className="input-group-btn">
                            <button type="submit" className="btn btn-primary">
                                <span>Search </span><i className="glyphicon glyphicon-search"></i>
                            </button>
                        </div>
                    </div>
                </form>
            </div>
        );
    }
});

var SearchResultList = React.createClass({
    render: function() {
        return (
            <div className="searchResultList">
                {this.props.data.map(function(link) {
                    return (
                        <div className="panel panel-default" key={link.id}>
                            <div className="panel-body">
                                <h3><a href={link.link}>{link.title}</a></h3>
                                <p>{link.snippet}</p>
                            </div>
                        </div>
                    );
                })}
            </div>
        );
    }
});

var SearchResults = React.createClass({
    //Sets initial state
    getInitialState: function() {
        return { links: initLinks }//from static/CONSTANTS
    },
    handleSearchSubmit: function(data) {
        $.ajax({
            url: '/get_links',
            method: 'POST',
            data: JSON.stringify({ 'query':data.searchText}, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function(data) {
                console.log(data);
                data = JSON.parse(data);
                this.setState({ links: data });
            }.bind(this),
            error: function(xhr, status, err) {
                console.error('/get_links', status, err.toString());
            }.bind(this)
        });
    },
    render: function() {
        return (
            <div className="searchResults">
                <SearchForm onSearchSubmit={this.handleSearchSubmit} />
                <SearchResultList data={this.state.links} />
            </div>
        );
    }
});

ReactDOM.render(
    <SearchResults />,
    document.getElementById('mountReact')
);

